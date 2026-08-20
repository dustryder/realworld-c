#include "main.h"
#include "../../lib/constants.h"
#include "../../data/tag.h"
#include "../../services/articles/articles_services.h"
#include "articles_handlers.h"

void handle_get_articles(http_s* h) {
    FIO_LOG_DEBUG("handle_get_profile");

    int id = parse_request_user(h->params);
    char *slug = parse_path_param(h->params, "slug");
    char *response_body;

    GetArticleResult result = get_article_by_slug(id, slug);

    response_body = create_article_success_response(result.result);

    h->status = HTTP_SUCCESS;

    http_send_body(h, response_body, strlen(response_body));
}