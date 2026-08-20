#include "main.h"
#include "cJSON.h"
#include "http_helpers.h"

char* get_bearer_token(http_s* h) {
  int BEARER_PREFIX_LENGTH = 6;

  FIOBJ authorization_header_key = fiobj_str_new("authorization", 13);
  FIOBJ auth = fiobj_hash_get(h->headers, authorization_header_key);

  if (FIOBJ_TYPE_IS(auth, FIOBJ_T_NULL)) {
    return NULL;
  }

  char *token = fiobj_obj2cstr(auth).data;
 
  char *buffer = malloc((strlen(token) - BEARER_PREFIX_LENGTH + 1) * sizeof(char));

  strncpy(buffer, token + BEARER_PREFIX_LENGTH, strlen(token) - BEARER_PREFIX_LENGTH);
  buffer[strlen(token) - BEARER_PREFIX_LENGTH] = '\0';

  return buffer;
}

char *parse_path_param(FIOBJ *params, char *key) {

    FIOBJ fiobj_key = fiobj_str_new(key, strlen(key));

    FIOBJ fiobj_value = fiobj_hash_get(params, fiobj_key);

    char* value = fiobj_obj2cstr(fiobj_value).data;

    return value;
}

int parse_request_user(FIOBJ *params) {
    FIOBJ fiobj_key = fiobj_str_new("_id", 3);
    FIOBJ fiobj_value = fiobj_hash_get(params, fiobj_key);
    char* value = fiobj_obj2cstr(fiobj_value).data;

    int converted = strtol(value, NULL, 10);

    return converted;
}

char *create_failure_body_from_errors(ErrorValue* errors, size_t error_count) {

  cJSON *response_body = cJSON_CreateObject();
  cJSON *error_body = cJSON_CreateObject();

  for (int i = 0; i < error_count; i++) {
    ErrorValue currentError = errors[i];

    if (cJSON_GetObjectItem(error_body, currentError.property) != NULL) {
      cJSON *errorArray = cJSON_GetObjectItem(error_body, currentError.property);
      cJSON *errorArrayItem = cJSON_CreateString(currentError.error);
      cJSON_AddItemToArray(errorArray, errorArrayItem);
    } else {
      cJSON *errorArray = cJSON_CreateArray();
      cJSON *errorArrayItem = cJSON_CreateString(currentError.error);
      cJSON_AddItemToArray(errorArray, errorArrayItem);
      cJSON_AddItemToObject(error_body, currentError.property, errorArray);
    }
  }

  cJSON_AddItemToObject(response_body, "errors", error_body);

  char *response_string = cJSON_Print(response_body);
  cJSON_Delete(response_body);

  return response_string;
}