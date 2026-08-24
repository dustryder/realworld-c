#include "main.h"
#include "../../lib/constants.h"
#include "../../services/articles/articles_services.h"

void handle_delete_articles(http_s* h) {
    FIO_LOG_DEBUG("handle_delete_articles");

    int id = parse_request_user(h->params);
    char *slug = parse_path_param(h->params, "slug");
    char *response_body = NULL;

    ArticleServiceResult result = delete_article(h->udata, slug, id);

    if (result.status == SERVICE_SUCCESS) {
        response_body = create_empty_response();
        h->status = HTTP_NO_CONTENT;
    } else if (result.status == SERVICE_NOT_FOUND) {
        response_body = create_failure_body_from_error(result.error);
        h->status = HTTP_NOT_FOUND;
    } else if (result.status == SERVICE_UNAUTHORIZED) {
        response_body = create_failure_body_from_error(result.error);
        h->status = HTTP_FORBIDDEN;
    } else {
        response_body = create_empty_response();
        h->status = HTTP_INTERNAL_SERVER_ERROR;
    }

    http_send_body(h, response_body, strlen(response_body));

    free(response_body);
    free(slug);
}