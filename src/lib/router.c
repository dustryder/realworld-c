#include "router.h" 
#include <stdbool.h>
#include "main.h"

bool http_path_matches(http_s *request, char *__route) {
    // make a mutable copy of the route
    char *route = strdup(__route);

    if (!route) {
        FIO_LOG_ERROR("Failed to allocate memory!");
        return false;
    }

    char *copied_path = strdup(fiobj_obj2cstr(request->path).data);

    // truncate the path at the query string delimiter,
    // as we only care about the path itself and the 
    // query string is parsed by http_parse_query()
    char *path = strtok(copied_path, "?");

    // does the route contain any inline path variables?
    if (strchr(route, ':') == NULL) {
        // no - perform direct string comparison
        if (strcmp(route, path) == 0) {
            free(route);
            free(copied_path);
            return true;
        } else {
            free(route);
            free(copied_path);
            return false;
        }
    }

    int route_part_cnt = 0;
    int path_part_cnt  = 0;

    // count the number of parts in the route and the path
    for (int i = 0; route[i]; route[i] == '/' ? route_part_cnt++, i++ : i++);
    for (int i = 0; path[i];  path[i]  == '/' ? path_part_cnt++,  i++ : i++);

    // do we have an equal number of parts?
    if (route_part_cnt != path_part_cnt) {
        free(copied_path);
        free(route);
        return false;
    }

    char *route_parts[route_part_cnt];
    char *path_parts[path_part_cnt];

    char *route_remaining;
    char *path_remaining;

    int matches = 0;

    // loop through each part

    char *route_next_part = strtok_r(route, "/", &route_remaining);
    char *path_next_part  = strtok_r(path,  "/", &path_remaining);

    while (route_next_part && path_next_part) {

        // if the route part is an inline variable, extract the variable name and its value
        if (route_next_part[0] == ':') {
            route_parts[matches]  = route_next_part + 1;
            path_parts[matches++] = path_next_part;
        } else {
            // the route part is literal, does it match the path part?
            if (strcmp(route_next_part, path_next_part)) {
                free(route);
                free(copied_path);
                return false;
            }
        }

        route_next_part = strtok_r(NULL, "/", &route_remaining);
        path_next_part  = strtok_r(NULL, "/", &path_remaining);
    }

    if (!FIOBJ_TYPE_IS(request->params, FIOBJ_T_HASH)) {
        request->params = fiobj_hash_new();
    }

    // add the inline path variable names and values to the request params
    for (int i = 0; i < matches; i++) {
        http_add2hash(request->params, route_parts[i], strlen(route_parts[i]), path_parts[i], strlen(path_parts[i]), 1);
    }

    free(route);
    free(copied_path);

    return true;
}