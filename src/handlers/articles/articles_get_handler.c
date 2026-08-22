#include "main.h"
#include "../../lib/constants.h"
#include "../../data/tag.h"
#include "../../services/articles/articles_services.h"
#include "articles_handlers.h"

void handle_get_articles(http_s* h) {
    FIO_LOG_DEBUG("handle_get_articles");

    int id = parse_request_user(h->params);
    char *slug = parse_path_param(h->params, "slug");
    char *response_body;

    GetArticleResult result = get_article_by_slug(h->udata, id, slug);

    if (result.status == SERVICE_SUCCESS) {
        response_body = create_article_success_response(result.result, true, FORMAT_DATETIMESTAMP);
        h->status = HTTP_SUCCESS;
    } else if (result.status == SERVICE_NOT_FOUND) {
        ErrorValue errors[1] = { result.error };
        response_body = create_failure_body_from_errors(errors, 1);
        h->status = HTTP_NOT_FOUND;
    }

    http_send_body(h, response_body, strlen(response_body));
}