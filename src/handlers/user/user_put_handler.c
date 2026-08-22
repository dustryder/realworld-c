#include "main.h"
#include "user_handlers.h"
#include "../../services/users/users_services.h"
#include "cJSON.h"
#include "../../lib/constants.h"
#include "../../lib/http_helpers.h"
#include "../../lib/token.h"

void handle_put_user(http_s* h) {

    char* response_body;
    int id = parse_request_user(h->params);

    ErrorValue errors[2];
    size_t error_count = 0;

    PutUserPayload values = parse_put_user_body(h->body);
    validate_put_user_payload(values, errors, &error_count);

    if (error_count > 0) {
      response_body = create_failure_body_from_errors(errors, error_count);
      h->status = HTTP_UNPROCESSABLE_ENTITY;
    } else {
      UpdateUserResult result = update_user(h->udata,
          id,
          values.email,
          values.password,
          values.username,
          values.bio,
          values.image
      );

      if (result.status == UPDATE_USER_SUCCESS) {
        response_body = create_user_success_response(
          result.result.email,
          result.result.username,
          result.result.token,
          result.result.bio,
          result.result.image
        );
        h->status = HTTP_SUCCESS;
      } else if (result.status == UPDATE_USER_FAILURE) {
        response_body = create_post_user_failure();
        h->status = HTTP_NOT_FOUND;
      }
    }

    http_send_body(h, response_body, strlen(response_body));
}

void validate_put_user_payload(PutUserPayload payload, ErrorValue *values, size_t *error_count) {

  if (payload.username.is_present == 1 && (payload.username.value == NULL || strlen(payload.username.value) == 0)) {
    values[*error_count].property = "username";
    values[*error_count].error = "can't be blank";
    (*error_count)++;
  }

  if (payload.email.is_present == 1 && (payload.email.value == NULL || strlen(payload.email.value) == 0)) {
    values[*error_count].property = "email";
    values[*error_count].error = "can't be blank";
    (*error_count)++;
  }

  if (payload.password.is_present == 1 && (payload.password.value == NULL || strlen(payload.password.value) == 0)) {
    values[*error_count].property = "password";
    values[*error_count].error = "can't be blank";
    (*error_count)++;
  }

  if (payload.password.is_present == 1 && (payload.password.value != NULL && strlen(payload.password.value) < 8)) {
    values[*error_count].property = "password";
    values[*error_count].error = "must be at least 8 characters";
    (*error_count)++;
  }
}

PutUserPayload parse_put_user_body(FIOBJ *raw_body) {

  FIOBJ user_key = fiobj_str_new("user", 4);

  char *body = fiobj_obj2cstr(raw_body).data;

  FIOBJ jsonBody = FIOBJ_INVALID;
  fiobj_json2obj(&jsonBody, body, strlen(body));

  FIOBJ user_body = fiobj_hash_get(jsonBody, user_key);

  PutUserPayload values;

  values.email = parse_optional_string(user_body, "email");
  values.password = parse_optional_string(user_body, "password");
  values.username = parse_optional_string(user_body, "username");
  values.bio = parse_optional_string(user_body, "bio");
  values.image = parse_optional_string(user_body, "image");

  fiobj_free(user_key);

  return values;
}