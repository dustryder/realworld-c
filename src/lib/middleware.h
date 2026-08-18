#include "main.h"
#include <stdlib.h>

#ifndef LIB_MIDDLEWARE_H
#define LIB_MIDDLEWARE_H
bool resolve_request_user(http_s *request);
bool require_auth(http_s *request);
#endif