#include "main.h"
#include "profile_handlers.h"
#include "../../services/profiles/profiles_services.h"
#include "../../lib/http_helpers.h"
#include "../../lib/constants.h"

void handle_get_profile(http_s* h) {
    FIO_LOG_DEBUG("handle_get_profile");

    char *username = parse_path_param(h->params, "username");
    int id = parse_request_user(h->params);
    char *response_body = NULL;

    ProfileServiceResult result = get_profile_by_username(h->udata, username, id);

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
