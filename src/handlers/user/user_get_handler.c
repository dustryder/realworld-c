#include "main.h"
#include "user_handlers.h"
#include "../../services/users/users_services.h"
#include "cJSON.h"
#include "../../lib/constants.h"
#include "../../lib/http_helpers.h"
#include "../../lib/token.h"

void handle_get_user(http_s* h) {

    int id = parse_request_user(h->params);

    char* response_body = NULL;

    UserServiceResult result = get_user_by_id(h->udata, id);

    if (result.status == SERVICE_SUCCESS) {
      response_body = create_user_success_response(
        result.data.email,
        result.data.username,
        result.data.token,
        result.data.bio,
        result.data.image
      );
      h->status = HTTP_SUCCESS;
    } else if (result.status == SERVICE_NOT_FOUND) {
      response_body = create_empty_response();
      h->status = HTTP_NOT_FOUND;
    } else {
      response_body = create_empty_response();
      h->status = HTTP_INTERNAL_SERVER_ERROR;
    }

    http_send_body(h, response_body, strlen(response_body));

    free(response_body);
    free_UserServiceResultData(&result.data);
}
