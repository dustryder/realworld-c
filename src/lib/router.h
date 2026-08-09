#define http_route_handler(_h, _method, _route, _func) {                                                \
    if (strcmp(fiobj_obj2cstr(_h->method).data, _method) == 0 && http_path_matches(_h, _route)) {  \
        FIO_LOG_DEBUG("Matched route %s %s", _method, _route);                                          \
        _func(_h);                                                                                      \
        return;                                                                                         \
    }                                                                                                   \
}

#define http_route_get(_h, _route, _func) \
    http_route_handler(_h, "GET", _route, _func)

#define http_route_post(_h, _route, _func) \
    http_route_handler(_h, "POST", _route, _func)

#define http_route_put(_h, _route, _func) \
    http_route_handler(_h, "PUT", _route, _func)

#define http_route_delete(_h, _route, _func) \
    http_route_handler(_h, "DELETE", _route, _func)