#include "main.h";
#include "../../lib/constants.h"
#include "comments_handlers.h";
#include "../../services/comments/comments_services.h"

void handle_get_comments(http_s *h) {

    char *response_body = "";
    char *slug = parse_path_param(h->params, "slug");

    AllCommentsServiceResult service_result = get_article_comments(h->udata, slug);

    if (service_result.status == SERVICE_SUCCESS) {
        h->status = HTTP_SUCCESS;
        response_body = create_many_comment_success_response(service_result.result, service_result.result_count);
    } else if (service_result.status == SERVICE_NOT_FOUND) {
        response_body = create_failure_body_from_error(service_result.error);
        h->status = HTTP_NOT_FOUND;
    }

    
    http_send_body(h, response_body, strlen(response_body));
}