#include "main.h"
#include "../../services/profiles/profiles_services.h"
#include "../../lib/constants.h"
#include "../../lib/http_helpers.h"
#include "profile_handlers.h"

void handle_delete_follow(http_s* h) {
    FIO_LOG_DEBUG("handle_delete_follow");
    char *token = get_bearer_token(h);
    char *username = parse_path_param(h->params, "username");

    char* response_body;

    if (token == NULL) {
      ErrorValue errors[1];
      errors[0].error = "is missing";
      errors[0].property = "token";

      response_body = create_failure_body_from_errors(errors, 1);
      h->status = HTTP_UNAUTHORIZED;
      http_send_body(h, response_body, strlen(response_body));
      return;
    }

    int id = decode_jwt_sub(token);

    free(token); 

    UnfollowUserResult result = unfollow_user(id, username);

    if (result.status == FOLLOW_USER_SUCCESS) {
      h->status = HTTP_SUCCESS;
      response_body = create_success_profile_response(result.result);
    } else if (result.status == FOLLOW_USER_UNKNOWN) {
      h->status = HTTP_NOT_FOUND;
      ErrorValue errors[1] = { result.error };
      response_body = create_failure_body_from_errors(errors, 1);
    }

    http_send_body(h, response_body, strlen(response_body));
}
