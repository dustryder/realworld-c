#include "main.h"
#include "user_handlers.h"
#include "../services/users/users_services.h"
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