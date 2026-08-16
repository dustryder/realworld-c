#include "main.h"
#include "user_handlers.h"
#include "../services/users/users_services.h"
#include "cJSON.h"
#include "../../lib/constants.h"
#include "../../lib/http_helpers.h"

void handle_post_user(http_s* h) {
    PostUserPayload values = parse_post_user_body(h->body);

    char *request_body;

    ErrorValue errors[3];
    size_t error_count = 0;

    validate_user_payload(values, errors, &error_count);

    if (error_count > 0) {
      request_body = create_post_user_failure_from_errors(errors, error_count);
      h->status = HTTP_UNPROCESSABLE_ENTITY;
    } else {
      RegisterUserStatus result = register_user(values.email, values.username, values.password);

      if (result.status == CREATE_USER_SUCCESS) {
        request_body = create_user_success_response(values.email, values.username, result.result, NULL, NULL);
        h->status = HTTP_CREATED;
      } else if (result.status == CREATE_USER_DUPLICATE) {
        ErrorValue errors[1] = { result.error };
        request_body = create_post_user_failure_from_errors(errors, 1);
        h->status = HTTP_CONFLICT;
      }
    }

    http_send_body(h, request_body, strlen(request_body));
}

void validate_user_payload(PostUserPayload payload, ErrorValue *values, size_t *error_count) {

  if (strlen(payload.username) == 0) {
    values[*error_count].property = "username";
    values[*error_count].error = "can't be blank";
    (*error_count)++;
  }

  if (strlen(payload.email) == 0) {
    values[*error_count].property = "email";
    values[*error_count].error = "can't be blank";
    (*error_count)++;
  }

  if (strlen(payload.password) == 0) {
    values[*error_count].property = "password";
    values[*error_count].error = "can't be blank";
    (*error_count)++;
  }
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