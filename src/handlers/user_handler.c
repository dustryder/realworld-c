#include "main.h"
#include "user_handler.h"
#include "../services/users_service.h"
#include "cJSON.h"

HttpResult handle_post_user(http_s* h) {

    PostUserPayload values = parse_post_user_body(h->body);
    HttpResult http_result;

    printf("I am here");

    RegisterUserStatus result = register_user(values.email, values.username, values.password);

    if (result.status == CREATE_USER_SUCCESS) {
      http_result.body = create_post_user_success(values.email, values.username, result.result);
      http_result.status = 201;
    } else if (result.status == CREATE_USER_DUPLICATE) {
      http_result.body = create_post_user_failure();
      http_result.status = 409;
    }

    h->status = http_result.status;

    http_send_body(h, http_result.body, strlen(http_result.body));
}

char* create_post_user_failure() {

  cJSON *arr = cJSON_CreateArray();
  cJSON *str = cJSON_CreateString("Oh no");
  cJSON_AddItemToArray(arr, str);

  cJSON *error = cJSON_CreateObject();
  cJSON_AddItemToObject(error, "username", arr);

  cJSON *res = cJSON_CreateObject();
  cJSON_AddItemToObject(res, "errors", error);

  const *response_string = cJSON_Print(res);

  return response_string;
}

char* create_post_user_success(char *email, char* username, char* token) {
    cJSON *wrapper = cJSON_CreateObject();
    cJSON *user_properties = cJSON_CreateObject();

    cJSON_AddItemToObject(user_properties, "email", cJSON_CreateString(email));
    cJSON_AddItemToObject(user_properties, "username", cJSON_CreateString(username));
    cJSON_AddItemToObject(user_properties, "token", cJSON_CreateString(token));
    cJSON_AddItemToObject(user_properties, "bio", cJSON_CreateNull());
    cJSON_AddItemToObject(user_properties, "image", cJSON_CreateNull());

    cJSON_AddItemToObject(wrapper, "user", user_properties);

    const *response_string = cJSON_Print(wrapper);

    return response_string;
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
