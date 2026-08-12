#include "main.h"
#include "user_handler.h"
#include "../services/users_service.h"
#include "cJSON.h"
#include "../lib/constants.h"

void handle_post_user(http_s* h) {
    PostUserPayload values = parse_post_user_body(h->body);
    HttpResult http_result;

    RegisterUserStatus result = register_user(values.email, values.username, values.password);

    if (result.status == CREATE_USER_SUCCESS) {
      http_result.body = create_user_success_response(values.email, values.username, result.result, NULL, NULL);
      http_result.status = HTTP_CREATED;
    } else if (result.status == CREATE_USER_DUPLICATE) {
      http_result.body = create_post_user_failure();
      http_result.status = HTTP_CONFLICT;
    }

    h->status = http_result.status;

    http_send_body(h, http_result.body, strlen(http_result.body));
}

char* get_bearer_token(http_s* h) {
  int BEARER_PREFIX_LENGTH = 6;

  FIOBJ authorization_header_key = fiobj_str_new("authorization", 13);
  FIOBJ auth = fiobj_hash_get(h->headers, authorization_header_key);
  char *token = fiobj_obj2cstr(auth).data;
 
  char *buffer = malloc((strlen(token) - BEARER_PREFIX_LENGTH + 1) * sizeof(char));

  strncpy(buffer, token + BEARER_PREFIX_LENGTH, strlen(token) - BEARER_PREFIX_LENGTH);
  buffer[strlen(token) - BEARER_PREFIX_LENGTH] = '\0';

  return buffer;
}

void handle_get_user(http_s* h) {

    char *token = get_bearer_token(h);
    char* body;

    if (strcmp(token, "null") == 0) {
      h->status = HTTP_UNAUTHORIZED;
      body = create_post_user_failure();
      http_send_body(h, body, strlen(body));
      return;
    }

    int id = decode_jwt_sub(token);
    free(token);

    GetUserByIdResult result = get_user_by_id(id);

    if (result.status == CREATE_USER_SUCCESS) {
      h->status = HTTP_SUCCESS;
      body = create_user_success_response(
        result.result.email,
        result.result.username,
        result.result.token,
        result.result.bio,
        result.result.image
      );
      h->status = HTTP_SUCCESS;
    } else if (result.status == CREATE_USER_DUPLICATE) {
      body = create_post_user_failure();
      h->status = HTTP_NOT_FOUND;
    }

    http_send_body(h, body, strlen(body));
}

void handle_post_login(http_s* h) {

    PostLoginRequestPayload values = parse_post_login_body(h->body);
    LoginUserResult result = login(values.email, values.password);
    char* body;

    if (result.status == LOGIN_USER_SUCCESS) {
      h->status = HTTP_SUCCESS;
      body = create_user_success_response(
        result.result.email,
        result.result.username,
        result.result.token,
        result.result.bio,
        result.result.image
      );
    } else if (result.status == LOGIN_USER_UNKNOWN || result.status == LOGIN_USER_BAD_PASSWORD) {
      h->status = HTTP_UNAUTHORIZED;
      body = "Potato";
    }
    http_send_body(h, body, strlen(body));
}

char* create_post_user_failure() {

  cJSON *arr = cJSON_CreateArray();
  cJSON *str = cJSON_CreateString("Oh no");
  cJSON_AddItemToArray(arr, str);

  cJSON *error = cJSON_CreateObject();
  cJSON_AddItemToObject(error, "username", arr);

  cJSON *res = cJSON_CreateObject();
  cJSON_AddItemToObject(res, "errors", error);

  char *response_string = cJSON_Print(res);

  return response_string;
}

char* create_user_success_response(char *email, char *username, char *token, char *bio, char *image) {
    cJSON *wrapper = cJSON_CreateObject();
    cJSON *user_properties = cJSON_CreateObject();

    cJSON_AddItemToObject(user_properties, "email", cJSON_CreateString(email));
    cJSON_AddItemToObject(user_properties, "username", cJSON_CreateString(username));
    cJSON_AddItemToObject(user_properties, "token", cJSON_CreateString(token));
    cJSON_AddItemToObject(user_properties, "bio", bio != NULL ? cJSON_CreateString(bio) : cJSON_CreateNull());
    cJSON_AddItemToObject(user_properties, "image", image != NULL ? cJSON_CreateString(image) : cJSON_CreateNull());

    cJSON_AddItemToObject(wrapper, "user", user_properties);

    char *response_string = cJSON_Print(wrapper);

    return response_string;
}


/** 
 * Request parsing
*/
PostLoginRequestPayload parse_post_login_body(FIOBJ *body) {
  
  PostLoginRequestPayload payload;
  FIOBJ user_key = fiobj_str_new("user", 4);

  FIOBJ email_key = fiobj_str_new("email", 5);
  FIOBJ password_key = fiobj_str_new("password", 8);

  char *parsed_body = fiobj_obj2cstr(body).data;

  FIOBJ json_body = FIOBJ_INVALID;
  fiobj_json2obj(&json_body, parsed_body, strlen(parsed_body));

  FIOBJ user_body = fiobj_hash_get(json_body, user_key);

  payload.email = fiobj_obj2cstr(fiobj_hash_get(user_body, email_key)).data;
  payload.password = fiobj_obj2cstr(fiobj_hash_get(user_body, password_key)).data;

  fiobj_free(user_key);
  fiobj_free(email_key);
  fiobj_free(password_key);

  return payload;
}

PostUserPayload parse_post_user_body(FIOBJ *raw_body) {

  FIOBJ user_key = fiobj_str_new("user", 4);

  FIOBJ username_key = fiobj_str_new("username", 8);
  FIOBJ email_key = fiobj_str_new("email", 5);
  FIOBJ password_key = fiobj_str_new("password", 8);

  char *body = fiobj_obj2cstr(raw_body).data;

  FIOBJ jsonBody = FIOBJ_INVALID;
  fiobj_json2obj(&jsonBody, body, strlen(body));

  FIOBJ user_body = fiobj_hash_get(jsonBody, user_key);

  PostUserPayload values;

  values.email = fiobj_obj2cstr(fiobj_hash_get(user_body, email_key)).data;
  values.password = fiobj_obj2cstr(fiobj_hash_get(user_body, password_key)).data;
  values.username = fiobj_obj2cstr(fiobj_hash_get(user_body, username_key)).data;

  fiobj_free(user_key);
  fiobj_free(username_key);
  fiobj_free(email_key);
  fiobj_free(password_key);

  return values;
}

