#include "main.h"
#include "../../lib/constants.h"
#include "../../services/comments/comments_services.h"
#include "comments_handlers.h"

void handle_delete_comments(http_s *h) {
    FIO_LOG_DEBUG("handle_delete_comments");

    int user_id = parse_request_user(h->params);
    int article_id = parse_path_param_number(h->params, "id");
    char *slug = parse_path_param(h->params, "slug");

    char *response_body = NULL;
    CommentsServiceResult service_result = delete_comment(h->udata, article_id, slug, user_id);

    if (service_result.status == SERVICE_SUCCESS) {
        response_body = create_empty_response();
        h->status = HTTP_NO_CONTENT;
    } else if (service_result.status == SERVICE_NOT_FOUND) {
        response_body = create_failure_body_from_error(service_result.error);
        h->status = HTTP_NOT_FOUND;
    } else if (service_result.status == SERVICE_UNAUTHORIZED) {
        response_body = create_failure_body_from_error(service_result.error);
        h->status = HTTP_FORBIDDEN;
    } else {
        response_body = create_empty_response();
        h->status = HTTP_INTERNAL_SERVER_ERROR;
    }

    http_send_body(h, response_body, strlen(response_body));

    free(response_body);
    free(slug);
}