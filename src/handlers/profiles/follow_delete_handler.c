#include "main.h"
#include "../../services/profiles/profiles_services.h"
#include "../../lib/constants.h"
#include "../../lib/http_helpers.h"
#include "profile_handlers.h"

void handle_delete_follow(http_s* h) {
    FIO_LOG_DEBUG("handle_delete_follow");

    char *username = parse_path_param(h->params, "username");
    int id = parse_request_user(h->params);

    char* response_body = NULL;

    ProfileServiceResult result = unfollow_user(h->udata, id, username);

    if (result.status == SERVICE_SUCCESS) {
      response_body = create_success_profile_response(result.result);
      h->status = HTTP_SUCCESS;
    } else if (result.status == SERVICE_NOT_FOUND) {
      response_body = create_failure_body_from_error(result.error);
      h->status = HTTP_NOT_FOUND;
    } else {
      response_body = create_empty_response();
      h->status = HTTP_INTERNAL_SERVER_ERROR;
    }

    http_send_body(h, response_body, strlen(response_body));

    free(response_body);
    free(username);
    free_ProfileServiceResultData(&result.result);
}
