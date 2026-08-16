#include "main.h"
#include "user_handlers.h"
#include "../../services/users/users_services.h"
#include "cJSON.h"
#include "../../lib/constants.h"
#include "../../lib/http_helpers.h"
#include "../../lib/token.h"

void handle_put_user(http_s* h) {

    char *token = get_bearer_token(h);

    char* response_body;

    if (token == NULL) {
      ErrorValue errors[1];
      errors[0].error = "is missing";
      errors[0].property = "token";

      response_body = create_post_user_failure_from_errors(errors, 1);
      h->status = HTTP_UNAUTHORIZED;
      http_send_body(h, response_body, strlen(response_body));
      return;
    }

    int id = decode_jwt_sub(token);

    free(token);

    ErrorValue errors[2];
    size_t error_count = 0;

    PutUserPayload values = parse_put_user_body(h->body);
    validate_put_user_payload(values, errors, &error_count);

    if (error_count > 0) {
      response_body = create_post_user_failure_from_errors(errors, error_count);
      h->status = HTTP_UNPROCESSABLE_ENTITY;
    } else {
      UpdateUserResult result = update_user(id,
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

  if (payload.username.is_present == 1 && payload.username.value == NULL || strlen(payload.username.value) == 0) {
    values[*error_count].property = "username";
    values[*error_count].error = "can't be blank";
    (*error_count)++;
  }

  if (payload.email.is_present == 1 && payload.email.value == NULL || strlen(payload.email.value) == 0) {
    values[*error_count].property = "email";
    values[*error_count].error = "can't be blank";
    (*error_count)++;
  }

  if (payload.password.is_present == 1 && payload.password.value == NULL || strlen(payload.password.value) == 0) {
    values[*error_count].property = "password";
    values[*error_count].error = "can't be blank";
    (*error_count)++;
  }

  if (payload.password.is_present == 1 && payload.password.value != NULL && strlen(payload.password.value) < 8) {
    values[*error_count].property = "password";
    values[*error_count].error = "must be at least 8 characters";
    (*error_count)++;
  }
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