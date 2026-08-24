#include "http.h"
#include "http_helpers.h"
#include "constants.h"
#include "token.h"
#include <stdbool.h>

bool resolve_request_user(http_s *request) {
    char *token = get_bearer_token(request);

    if (!FIOBJ_TYPE_IS(request->params, FIOBJ_T_HASH)) {
        request->params = fiobj_hash_new();
    }

    if (token == NULL) {
        FIOBJ user_id_key = fiobj_str_new("_id", 3);
        fiobj_hash_set(request->params, user_id_key, fiobj_null());
    } else {
        int id = decode_jwt_sub(token);
        free(token);

        char str_id[20];
        sprintf(str_id, "%d", id);

        http_add2hash(request->params, "_id", 3, str_id, strlen(str_id), 1);
    }

    return true;
}

bool require_auth(http_s *request) {

    if (FIOBJ_TYPE_IS(request->params, FIOBJ_T_HASH)) {
        FIOBJ user_id_key = fiobj_str_new("_id", 3);
        FIOBJ user_id = fiobj_hash_get(request->params, user_id_key);

        if (FIOBJ_TYPE_IS(user_id, FIOBJ_T_NULL)) {
            ErrorValue errors[1];
            errors[0].message = "is missing";
            errors[0].property = "token";

            char *response_body = create_failure_body_from_errors(errors, 1);
            request->status = HTTP_UNAUTHORIZED;
            http_send_body(request, response_body, strlen(response_body));

            free(response_body);

            return false;
        }
    }

    return true;
}