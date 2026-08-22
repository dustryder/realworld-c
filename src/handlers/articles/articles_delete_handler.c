#include "main.h"
#include "../../lib/constants.h"
#include "../../services/articles/articles_services.h"

void handle_delete_articles(http_s* h) {
    FIO_LOG_DEBUG("handle_delete_articles");

    int id = parse_request_user(h->params);
    char *slug = parse_path_param(h->params, "slug");
    char *response_body;

    ArticleServiceResult result = delete_article(h->udata, slug);

    if (result.status == GetArticleSuccess) {
        h->status = HTTP_NO_CONTENT;
        response_body = "";
    } else if (result.status == GET_ARTICLE_UNKNOWN) {
        ErrorValue errors[1] = { result.error };
        response_body = create_failure_body_from_errors(errors, 1);
        h->status = HTTP_NOT_FOUND;
    }

    http_send_body(h, response_body, strlen(response_body));
}