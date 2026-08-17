#include "main.h"
#include "user_handlers.h"
#include "../../services/users/users_services.h"
#include "cJSON.h"
#include "../../lib/constants.h"

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

void handle_post_login(http_s* h) {

    PostLoginRequestPayload values = parse_post_login_body(h->body);

    ErrorValue errors[3];
    size_t error_count = 0;
    char *request_body;
    validate_post_login_payload(values, errors, &error_count);

    if (error_count > 0) {
      request_body = create_failure_body_from_errors(errors, error_count);
      h->status = HTTP_UNPROCESSABLE_ENTITY;
    } else {
      LoginUserResult result = login(values.email, values.password);

      if (result.status == LOGIN_USER_SUCCESS) {
        h->status = HTTP_SUCCESS;
        request_body = create_user_success_response(
          result.result.email,
          result.result.username,
          result.result.token,
          result.result.bio,
          result.result.image
        );
      } else if (result.status == LOGIN_USER_UNKNOWN || result.status == LOGIN_USER_BAD_PASSWORD) {
        h->status = HTTP_UNAUTHORIZED;
        ErrorValue errors[1] = { result.error };
        request_body = create_failure_body_from_errors(errors, 1);
      }
    }
    http_send_body(h, request_body, strlen(request_body));
}

void validate_post_login_payload(PostLoginRequestPayload payload, ErrorValue *values, size_t *error_count) {

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