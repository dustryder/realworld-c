#include "http.h"
#include "articles_handlers.h"
#include "../../services/articles/articles_services.h"
#include "../../lib/constants.h"

static GetAllArticleQuery parse_get_all_articles_qs(FIOBJ *params);

void handle_get_all_articles(http_s* h) {
    FIO_LOG_DEBUG("handle_get_all_profile");

    int id = parse_request_user(h->params);
    GetAllArticleQuery qs = parse_get_all_articles_qs(h->params);
    PGconn *conn = h->udata;

    char *response_body;

    GetAllArticleResult result = query_articles(conn, qs.author);

    response_body = create_many_article_success_response(result.result, result.article_count);
    h->status = HTTP_SUCCESS;

    http_send_body(h, response_body, strlen(response_body));
}

GetAllArticleQuery parse_get_all_articles_qs(FIOBJ *params) {

    GetAllArticleQuery qs;

    FIOBJ author_key = fiobj_str_new("author", 6);

    FIOBJ fio_author = fiobj_hash_get(params, author_key);

    qs.author = FIOBJ_TYPE_IS(fio_author, FIOBJ_T_STRING) ? fiobj_obj2cstr(fio_author).data : NULL;

    fiobj_free(author_key);

    return qs;
}