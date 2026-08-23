#include "articles_handlers.h"
#include "../../lib/constants.h"

void handle_post_favorite(http_s* h) {
    FIO_LOG_DEBUG("handle_get_articles");

    int id = parse_request_user(h->params);
    char *slug = parse_path_param(h->params, "slug");
    char *response_body = "";

    ArticleServiceResult result = favorite_article(h->udata, id, slug);

    if (result.status == SERVICE_SUCCESS) {
        h->status = HTTP_SUCCESS;
        response_body = create_article_success_response(result.result, true, FORMAT_DATETIMESTAMP);
    } else if (result.status == SERVICE_NOT_FOUND) {
        response_body = create_failure_body_from_error(result.error);
        h->status = HTTP_NOT_FOUND;
    }

    http_send_body(h, response_body, strlen(response_body));
}