#include "main.h"
#include "user_handlers.h"
#include "../../services/users/users_services.h"
#include "cJSON.h"
#include "../../lib/constants.h"
#include "../../lib/validate.h"

static void validate_PostLoginRequestPayload(PostLoginRequestPayload payload, ErrorValue *values, size_t *error_count);
static PostLoginRequestPayload parse_PostLoginRequestPayload(FIOBJ body);
void free_PostLoginRequestPayload(PostLoginRequestPayload payload);

void handle_post_login(http_s* h) {

    PostLoginRequestPayload values = parse_PostLoginRequestPayload(h->body);

    ErrorValue errors[3];
    size_t error_count = 0;
    char *response_body = NULL;
    validate_PostLoginRequestPayload(values, errors, &error_count);

    if (error_count > 0) {
      response_body = create_failure_body_from_errors(errors, error_count);
      h->status = HTTP_UNPROCESSABLE_ENTITY;
    } else {
      UserServiceResult result = login(h->udata, values.email.value, values.password.value);

      if (result.status == SERVICE_SUCCESS) {
        response_body = create_user_success_response(
          result.data.email,
          result.data.username,
          result.data.token,
          result.data.bio,
          result.data.image
        );
        h->status = HTTP_SUCCESS;
      } else if (result.status == SERVICE_NOT_FOUND || result.status == SERVICE_UNAUTHORIZED) {
        response_body = create_failure_body_from_error(result.error);
        h->status = HTTP_UNAUTHORIZED;
      } else {
        response_body = create_empty_response();
        h->status = HTTP_INTERNAL_SERVER_ERROR;
      }

      free_UserServiceResultData(&result.data);
    }

    http_send_body(h, response_body, strlen(response_body));

    free(response_body);
    free_PostLoginRequestPayload(values);
}

void free_PostLoginRequestPayload(PostLoginRequestPayload payload) {
  free(payload.email.value);
  free(payload.password.value);
}

void validate_PostLoginRequestPayload(PostLoginRequestPayload payload, ErrorValue *values, size_t *error_count) {

  is_required(payload.email, &values[*error_count], error_count, "email");
  is_required(payload.password, &values[*error_count], error_count, "password");

  not_null_or_empty(payload.email, &values[*error_count], error_count, "email");
  not_null_or_empty(payload.password, &values[*error_count], error_count, "password");
}

PostLoginRequestPayload parse_PostLoginRequestPayload(FIOBJ body) {
  
  PostLoginRequestPayload payload;
  FIOBJ user_key = fiobj_str_new("user", 4);

  char *parsed_body = fiobj_obj2cstr(body).data;

  FIOBJ json_body = FIOBJ_INVALID;
  fiobj_json2obj(&json_body, parsed_body, strlen(parsed_body));

  FIOBJ user_body = fiobj_hash_get(json_body, user_key);

  payload.email = parse_optional_string(user_body, "email");
  payload.password = parse_optional_string(user_body, "password");

  fiobj_free(user_key);

  return payload;
}