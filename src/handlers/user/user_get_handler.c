#include "main.h"
#include "user_handlers.h"
#include "../../services/users/users_services.h"
#include "cJSON.h"
#include "../../lib/constants.h"
#include "../../lib/http_helpers.h"
#include "../../lib/token.h"

void handle_get_user(http_s* h) {

    char *token = get_bearer_token(h);
    char* body;

    if (strcmp(token, "null") == 0) {
      h->status = HTTP_UNAUTHORIZED;
      body = create_post_user_failure();
      http_send_body(h, body, strlen(body));
      return;
    }

    int id = decode_jwt_sub(token);
    free(token);

    GetUserByIdResult result = get_user_by_id(id);

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
