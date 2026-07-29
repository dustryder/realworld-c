#include "main.h"
#include "user_handler.h"
#include "../services/users_service.h"

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

void handle_post_user(http_s* h) {

    PostUserPayload values = parse_post_user_body(h->body);

    createUser(values.email, values.username, values.password);
}