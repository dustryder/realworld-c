#include "main.h"
#include "user_handlers.h"
#include "../../services/users/users_services.h"
#include "cJSON.h"
#include "../../lib/constants.h"
#include "../../lib/http_helpers.h"
#include "../../lib/token.h"
#include "../../lib/validate.h"

static void free_PutUserPayload(PutUserPayload values);
static void validate_PutUserPayload(PutUserPayload payload, ErrorValue *values, size_t *error_count);
static PutUserPayload parse_PutUserPayload(FIOBJ *raw_body);

void handle_put_user(http_s* h) {

    char* response_body = NULL;
    int id = parse_request_user(h->params);

    ErrorValue errors[2];
    size_t error_count = 0;

    PutUserPayload values = parse_PutUserPayload(h->body);
    validate_PutUserPayload(values, errors, &error_count);

    if (error_count > 0) {
      response_body = create_failure_body_from_errors(errors, error_count);
      h->status = HTTP_UNPROCESSABLE_ENTITY;
    } else {
      UserServiceResult result = update_user(h->udata,
          id,
          values.email,
          values.password,
          values.username,
          values.bio,
          values.image
      );

      if (result.status == SERVICE_SUCCESS) {
        response_body = create_user_success_response(
          result.data.email,
          result.data.username,
          result.data.token,
          result.data.bio,
          result.data.image
        );
        h->status = HTTP_SUCCESS;

        free_UserServiceResultData(&result.data);
      } else if (result.status == SERVICE_NOT_FOUND) {
        response_body = create_empty_response();
        h->status = HTTP_NOT_FOUND;
      } else {
        response_body = create_empty_response();
        h->status = HTTP_INTERNAL_SERVER_ERROR;
      }
    }

    http_send_body(h, response_body, strlen(response_body));

    free(response_body);
    free_PutUserPayload(values);
}

void free_PutUserPayload(PutUserPayload values) {
    free(values.email.value);
    free(values.password.value);
    free(values.username.value);
    free(values.bio.value);
    free(values.image.value);
}

void validate_PutUserPayload(PutUserPayload payload, ErrorValue *values, size_t *error_count) {

  not_null_or_empty(payload.username, &values[*error_count], error_count, "username");
  not_null_or_empty(payload.email, &values[*error_count], error_count, "email");
  not_null_or_empty(payload.password, &values[*error_count], error_count, "password");

  if (payload.password.is_present == 1 && (payload.password.value != NULL && strlen(payload.password.value) < 8)) {
    values[*error_count].property = "password";
    values[*error_count].message = "must be at least 8 characters";
    (*error_count)++;
  }
}

PutUserPayload parse_PutUserPayload(FIOBJ *raw_body) {

  FIOBJ user_key = fiobj_str_new("user", 4);

  char *body = fiobj_obj2cstr(raw_body).data;

  FIOBJ json_body = FIOBJ_INVALID;
  fiobj_json2obj(&json_body, body, strlen(body));

  FIOBJ user_body = fiobj_hash_get(json_body, user_key);

  PutUserPayload values;

  values.email = parse_optional_string(user_body, "email");
  values.password = parse_optional_string(user_body, "password");
  values.username = parse_optional_string(user_body, "username");
  values.bio = parse_optional_string(user_body, "bio");
  values.image = parse_optional_string(user_body, "image");

  fiobj_free(user_key);
  fiobj_free(json_body);

  return values;
}