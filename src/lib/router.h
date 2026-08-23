#include <stdbool.h>
#include "main.h"
#include <stdarg.h>

bool http_path_matches(http_s *request, char *__route);

typedef bool (*middleware_fn)(http_s *request);

#define http_route_handler(_h, _method, _route, _func, ...) {                                           \
    if (strcmp(fiobj_obj2cstr(_h->method).data, _method) == 0 && http_path_matches(_h, _route)) {       \
        FIO_LOG_DEBUG("Matched route %s %s", _method, _route);                                          \
        middleware_fn middleware[] = { __VA_ARGS__ };                                                   \
        size_t middleware_count = sizeof(middleware) / sizeof(middleware[0]);                           \
                                                                                                        \
        for (size_t i = 0; i < middleware_count; i++) {                                                 \
            if (!middleware[i](_h)) {                                                                   \
                goto db_cleanup;                                                                        \
            }                                                                                           \
        }                                                                                               \
                                                                                                        \
        _func(_h);                                                                                      \
        goto db_cleanup;                                                                                \
    }                                                                                                   \
}

#define http_route_get(_h, _route, _func, ...) \
    http_route_handler(_h, "GET", _route, _func, __VA_ARGS__)

#define http_route_post(_h, _route, _func, ...) \
    http_route_handler(_h, "POST", _route, _func, __VA_ARGS__)

#define http_route_put(_h, _route, _func, ...) \
    http_route_handler(_h, "PUT", _route, _func, __VA_ARGS__)

#define http_route_delete(_h, _route, _func, ...) \
    http_route_handler(_h, "DELETE", _route, _func, __VA_ARGS__)
