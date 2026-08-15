#include "main.h"
#include "user_handlers.h"
#include "../../services/users/users_services.h"
#include "cJSON.h"
#include "../../lib/constants.h"
#include "../../lib/http_helpers.h"
#include "../../lib/token.h"

void handle_put_user(http_s* h) {

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

    PutUserPayload values = parse_put_user_body(h->body);

    UpdateUserResult result = update_user(id,
        values.email,
        values.password,
        values.username,
        values.bio,
        values.image
    );

    if (result.status == UPDATE_USER_SUCCESS) {
      h->status = HTTP_SUCCESS;
      body = create_user_success_response(
        result.result.email,
        result.result.username,
        result.result.token,
        result.result.bio,
        result.result.image
      );
      h->status = HTTP_SUCCESS;
    } else if (result.status == UPDATE_USER_FAILURE) {
      body = create_post_user_failure();
      h->status = HTTP_NOT_FOUND;
    }

    http_send_body(h, body, strlen(body));
}

OptionalValue parse_optional_value(FIOBJ obj, char* key) {
  FIOBJ fiobj_key = fiobj_str_new(key, strlen(key));
  OptionalValue optional_value;

  if (!fiobj_hash_haskey(obj, fiobj_key)) {
    optional_value.is_present = 0;
    return optional_value;
  }

  FIOBJ value = fiobj_hash_get(obj, fiobj_key);

  if (FIOBJ_TYPE_IS(value, FIOBJ_T_NULL)) {
    optional_value.is_present = 1;
    optional_value.value = NULL;
    return optional_value;
  }

  char *json_value = fiobj_obj2cstr(fiobj_hash_get(obj, fiobj_key)).data;
  optional_value.is_present = 1;
  optional_value.value = json_value;

  fiobj_free(fiobj_key);

  return optional_value;
}

PutUserPayload parse_put_user_body(FIOBJ *raw_body) {

  FIOBJ user_key = fiobj_str_new("user", 4);

  FIOBJ username_key = fiobj_str_new("username", 8);
  FIOBJ email_key = fiobj_str_new("email", 5);
  FIOBJ password_key = fiobj_str_new("password", 8);
  FIOBJ bio_key = fiobj_str_new("bio", 3);
  FIOBJ image_key = fiobj_str_new("image", 5);

  char *body = fiobj_obj2cstr(raw_body).data;

  FIOBJ jsonBody = FIOBJ_INVALID;
  fiobj_json2obj(&jsonBody, body, strlen(body));

  FIOBJ user_body = fiobj_hash_get(jsonBody, user_key);

  PutUserPayload values;

  values.email = parse_optional_value(user_body, "email");
  values.password = parse_optional_value(user_body, "password");
  values.username = parse_optional_value(user_body, "username");
  values.bio = parse_optional_value(user_body, "bio");
  values.image = parse_optional_value(user_body, "image");

  fiobj_free(user_key);

  return values;
}