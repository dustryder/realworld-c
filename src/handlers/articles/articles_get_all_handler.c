#include "http.h"
#include "articles_handlers.h"
#include "../../services/articles/articles_services.h"
#include "../../lib/constants.h"

void handle_get_all_articles(http_s* h) {
    FIO_LOG_DEBUG("handle_get_all_profile");

    int id = parse_request_user(h->params);
    PGconn *conn = h->udata;

    char *response_body;

    GetAllArticleResult result = query_articles(conn);

    response_body = create_many_article_success_response(result.result, result.article_count);
    h->status = HTTP_SUCCESS;

    http_send_body(h, response_body, strlen(response_body));
}