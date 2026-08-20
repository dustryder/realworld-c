#include "main.h"
#include "user_handlers.h"
#include "../../services/users/users_services.h"
#include "cJSON.h"
#include "../../lib/constants.h"
#include "../../lib/http_helpers.h"
#include "../../lib/token.h"

void handle_get_user(http_s* h) {

    char* body;

    int id = parse_request_user(h->params);

    GetUserByIdResult result = get_user_by_id(h->udata, id);

    if (result.status == CREATE_USER_SUCCESS) {
      h->status = HTTP_SUCCESS;
      body = create_user_success_response(
        result.result.email,
        result.result.username,
        result.result.token,
        result.result.bio,
        result.result.image
      );
      h->status = HTTP_SUCCESS;
    } else if (result.status == CREATE_USER_DUPLICATE) {
      body = create_post_user_failure();
      h->status = HTTP_NOT_FOUND;
    }

    http_send_body(h, body, strlen(body));
}
