#include "main.h"

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