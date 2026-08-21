#include "main.h"
#include "../../lib/constants.h"
#include "../../services/articles/articles_services.h"

void handle_delete_articles(http_s* h) {
    FIO_LOG_DEBUG("handle_delete_articles");

    int id = parse_request_user(h->params);
    char *slug = parse_path_param(h->params, "slug");
    char *response_body;

    delete_article(h->udata, slug);

    h->status = HTTP_NO_CONTENT;

    http_send_body(h, "", 0);
}