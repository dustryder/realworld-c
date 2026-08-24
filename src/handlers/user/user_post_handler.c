#include "main.h"
#include "user_handlers.h"
#include "../../services/users/users_services.h"
#include "cJSON.h"
#include "../../lib/constants.h"
#include "../../lib/http_helpers.h"
#include "../../lib/validate.h"

static void validate_PostUserPayload(PostUserPayload payload, ErrorValue *values, size_t *error_count);
static PostUserPayload parse_PostUserPayload(FIOBJ *raw_body);
static void free_PostUserPayload(PostUserPayload values);

void handle_post_user(http_s* h) {

    PostUserPayload values = parse_PostUserPayload(h->body);

    ErrorValue errors[3];
    size_t error_count = 0;
    validate_PostUserPayload(values, errors, &error_count);

    char *response_body = NULL;

    if (error_count > 0) {
      response_body = create_failure_body_from_errors(errors, error_count);
      h->status = HTTP_UNPROCESSABLE_ENTITY;
    } else {
      RegisterUserServiceResult result = register_user(h->udata, values.email.value, values.username.value, values.password.value);

      if (result.status == SERVICE_SUCCESS) {
        response_body = create_user_success_response(values.email.value, values.username.value, result.data, NULL, NULL);
        h->status = HTTP_CREATED;
      } else if (result.status == SERVICE_DUPLICATE) {
        response_body = create_failure_body_from_error(result.error);
        h->status = HTTP_CONFLICT;
      } else {
        response_body = create_empty_response();
        h->status = HTTP_INTERNAL_SERVER_ERROR;
      }

      free(result.data);
    }

    http_send_body(h, response_body, strlen(response_body));

    free_PostUserPayload(values);
    free(response_body);
}

void free_PostUserPayload(PostUserPayload values) {

    free(values.email.value);
    free(values.username.value);
    free(values.password.value);
}

void validate_PostUserPayload(PostUserPayload payload, ErrorValue *values, size_t *error_count) {

  is_required(payload.username, &values[*error_count], error_count, "username");
  is_required(payload.email, &values[*error_count], error_count, "email");
  is_required(payload.password, &values[*error_count], error_count, "password");

  not_null_or_empty(payload.username, &values[*error_count], error_count, "username");
  not_null_or_empty(payload.email, &values[*error_count], error_count, "email");
  not_null_or_empty(payload.password, &values[*error_count], error_count, "password");
}

PostUserPayload parse_PostUserPayload(FIOBJ *raw_body) {

  FIOBJ user_key = fiobj_str_new("user", 4);

  char *body = fiobj_obj2cstr(raw_body).data;

  FIOBJ json_body = FIOBJ_INVALID;
  fiobj_json2obj(&json_body, body, strlen(body));

  FIOBJ user_body = fiobj_hash_get(json_body, user_key);

  PostUserPayload values;

  values.email = parse_optional_string(user_body, "email");
  values.password = parse_optional_string(user_body, "password");
  values.username = parse_optional_string(user_body, "username");

  fiobj_free(user_key);
  fiobj_free(json_body);

  return values;
}