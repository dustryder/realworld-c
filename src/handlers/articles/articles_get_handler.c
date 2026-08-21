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

    GetArticleResult result = get_article_by_slug(h->udata, id, slug);

    if (result.status == GetArticleSuccess) {
        response_body = create_article_success_response(result.result, true, FORMAT_DATETIMESTAMP);
        h->status = HTTP_SUCCESS;
    } else if (result.status == GET_ARTICLE_UNKNOWN) {
        h->status = HTTP_NOT_FOUND;
        response_body = "Potato";
    }

    http_send_body(h, response_body, strlen(response_body));
}