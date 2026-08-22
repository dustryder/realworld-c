#include "main.h";
#include "../../lib/constants.h"
#include "../../services/comments/comments_services.h"
#include "comments_handlers.h";

void handle_delete_comments(http_s *h) {
    FIO_LOG_DEBUG("handle_delete_comments");

    int id = parse_path_param_number(h->params, "id");
    char *slug = parse_path_param(h->params, "slug");

    char *response_body = "";
    CommentsServiceResult service_result = delete_comment(h->udata, id, slug);

    if (service_result.status == SUCCESS) {
        h->status = HTTP_NO_CONTENT;
    } else if (service_result.status == NOT_FOUND) {
        h->status = HTTP_NOT_FOUND;
        response_body = create_failure_body_from_error(service_result.error);
    }

    http_send_body(h, response_body, strlen(response_body));
}