#include "main.h"
#include "profile_handlers.h"

void handle_get_profile(http_s* h) {

    char *username = parse_path_param(h->params, "username");
    
    printf("%s\n", username);
}

char *parse_path_param(FIOBJ *params, char *key) {

    // char *raw_params = fiobj_obj2cstr(params).data;

    FIOBJ fiobj_key = fiobj_str_new(key, strlen(key));

    FIOBJ fiobj_value = fiobj_hash_get(params, fiobj_key);

    char* value = fiobj_obj2cstr(fiobj_value).data;

    return value;
}