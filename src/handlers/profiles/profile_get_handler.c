#include "main.h"
#include "profile_handlers.h"
#include "../../services/profiles/profiles_services.h"
#include "../../lib/http_helpers.h"
#include "../../lib/constants.h"

void handle_get_profile(http_s* h) {
    FIO_LOG_DEBUG("handle_get_profile");

    char *username = parse_path_param(h->params, "username");

    GetProfileByUsernameResult result = get_profile_by_username(h->udata, username);
    char *response_body;

    if (result.status == GET_PROFILE_SUCCESS) {
      h->status = HTTP_SUCCESS;
      response_body = create_success_profile_response(result.result);
    } else if (result.status == GET_PROFILE_UNKNOWN) {
      h->status = HTTP_NOT_FOUND;
      ErrorValue errors[1] = { result.error };
      response_body = create_failure_body_from_errors(errors, 1);
    }

    http_send_body(h, response_body, strlen(response_body));
}
