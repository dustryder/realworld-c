#include "main.h"

char* get_bearer_token(http_s* h) {
  int BEARER_PREFIX_LENGTH = 6;

  FIOBJ authorization_header_key = fiobj_str_new("authorization", 13);
  FIOBJ auth = fiobj_hash_get(h->headers, authorization_header_key);
  char *token = fiobj_obj2cstr(auth).data;
 
  char *buffer = malloc((strlen(token) - BEARER_PREFIX_LENGTH + 1) * sizeof(char));

  strncpy(buffer, token + BEARER_PREFIX_LENGTH, strlen(token) - BEARER_PREFIX_LENGTH);
  buffer[strlen(token) - BEARER_PREFIX_LENGTH] = '\0';

  return buffer;
}
