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

void add_field(UpdateValue *values, size_t *count, char *key, OptionalValue value) {

    if (!value.is_present) {
        return;
    }

    values[*count].key = key;

    if (value.value == NULL) {
        values[*count].value = NULL;
    } else {
        values[*count].value = strcmp(value.value, "") != 0 ? value.value : NULL;
    }

    (*count)++;
}

OptionalArray parse_optional_array(FIOBJ obj, char* key) {
  FIOBJ fiobj_key = fiobj_str_new(key, strlen(key));
  OptionalArray optional_value;

  if (!fiobj_hash_haskey(obj, fiobj_key)) {
    optional_value.is_present = 0;
    optional_value.value = NULL;
    optional_value.value_count = 0;
    return optional_value;
  }

  FIOBJ value = fiobj_hash_get(obj, fiobj_key);

  if (FIOBJ_TYPE_IS(value, FIOBJ_T_NULL)) {
    optional_value.is_present = 1;
    optional_value.value = NULL;
    optional_value.value_count = 0;
    return optional_value;
  }

  char** result_value;

  optional_value.is_present = 1;
  if (FIOBJ_TYPE_IS(value, FIOBJ_T_ARRAY)) {

    result_value = malloc(sizeof(char *) * fiobj_ary_count(value));

    for (int i = 0; i < (int) fiobj_ary_count(value); i++) {
        FIOBJ item = fiobj_ary_index(value, i);
        char *string_item = strdup(fiobj_obj2cstr(item).data);
        result_value[i] = string_item;
    }

    optional_value.value = result_value;
    optional_value.value_count = fiobj_ary_count(value);
  }

  fiobj_free(fiobj_key);

  return optional_value;
}

OptionalValue parse_optional_string(FIOBJ obj, char* key) {
  FIOBJ fiobj_key = fiobj_str_new(key, strlen(key));
  OptionalValue optional_value;

  if (!fiobj_hash_haskey(obj, fiobj_key)) {
    optional_value.is_present = 0;
    optional_value.value = NULL;
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
  optional_value.value = strdup(json_value);

  fiobj_free(fiobj_key);

  return optional_value;
}

char *parse_path_param(FIOBJ *params, char *key) {

    FIOBJ fiobj_key = fiobj_str_new(key, strlen(key));

    FIOBJ fiobj_value = fiobj_hash_get(params, fiobj_key);

    char* value = strdup(fiobj_obj2cstr(fiobj_value).data);

    fiobj_free(fiobj_key);

    return value;
}

int parse_path_param_number(FIOBJ *params, char *key) {
    FIOBJ fiobj_key = fiobj_str_new(key, strlen(key));

    FIOBJ fiobj_value = fiobj_hash_get(params, fiobj_key);

    int value = fiobj_obj2num(fiobj_value);

    fiobj_free(fiobj_key);

    return value;
}

int parse_request_user(FIOBJ *params) {
    FIOBJ fiobj_key = fiobj_str_new("_id", 3);
    FIOBJ fiobj_value = fiobj_hash_get(params, fiobj_key);

    if (FIOBJ_TYPE_IS(fiobj_value, FIOBJ_T_NULL)) {
      fiobj_free(fiobj_key);
      return NULL;
    }
  
    char* value = fiobj_obj2cstr(fiobj_value).data;

    int converted = strtol(value, NULL, 10);

    fiobj_free(fiobj_key);

    return converted;
}

char *create_empty_response() {
  return strdup("");
}

char *create_failure_body_from_error(ErrorValue error) {
    ErrorValue errors[1] = { error };
    return create_failure_body_from_errors(errors, 1);
}

char *create_failure_body_from_errors(ErrorValue* errors, size_t error_count) {

  cJSON *response_body = cJSON_CreateObject();
  cJSON *error_body = cJSON_CreateObject();

  for (int i = 0; i < error_count; i++) {
    ErrorValue current_error = errors[i];

    if (cJSON_GetObjectItem(error_body, current_error.property) != NULL) {
      cJSON *errorArray = cJSON_GetObjectItem(error_body, current_error.property);
      cJSON *errorArrayItem = cJSON_CreateString(current_error.message);
      cJSON_AddItemToArray(errorArray, errorArrayItem);
    } else {
      cJSON *errorArray = cJSON_CreateArray();
      cJSON *errorArrayItem = cJSON_CreateString(current_error.message);
      cJSON_AddItemToArray(errorArray, errorArrayItem);
      cJSON_AddItemToObject(error_body, current_error.property, errorArray);
    }
  }

  cJSON_AddItemToObject(response_body, "errors", error_body);

  char *response_string = cJSON_Print(response_body);
  cJSON_Delete(response_body);

  return response_string;
}