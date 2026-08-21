#include "http.h"
#include "articles_handlers.h"
#include "../../services/articles/articles_services.h"
#include "../../lib/constants.h"

static GetAllArticleQuery parse_get_all_articles_qs(FIOBJ *params);

void handle_get_all_articles(http_s* h) {
    FIO_LOG_DEBUG("handle_get_all_articles");

    int id = parse_request_user(h->params);
    GetAllArticleQuery qs = parse_get_all_articles_qs(h->params);
    PGconn *conn = h->udata;

    char *response_body;

    GetAllArticleResult result = query_articles(conn, qs.author, qs.tag, qs.limit, qs.offset);

    printf("Article count handler: %d\n", result.article_count);

    response_body = create_many_article_success_response(result.result, result.article_count, result.total_count);
    h->status = HTTP_SUCCESS;

    http_send_body(h, response_body, strlen(response_body));
}

GetAllArticleQuery parse_get_all_articles_qs(FIOBJ *params) {

    GetAllArticleQuery qs;

    FIOBJ author_key = fiobj_str_new("author", 6);
    FIOBJ tag_key = fiobj_str_new("tag", 3);
    FIOBJ limit_key = fiobj_str_new("limit", 5);
    FIOBJ offset_key = fiobj_str_new("offset", 6);

    FIOBJ fio_author = fiobj_hash_get(params, author_key);
    FIOBJ fio_tag = fiobj_hash_get(params, tag_key);
    FIOBJ fio_limit = fiobj_hash_get(params, limit_key);
    FIOBJ fio_offset = fiobj_hash_get(params, offset_key);

    qs.author = FIOBJ_TYPE_IS(fio_author, FIOBJ_T_STRING) ? fiobj_obj2cstr(fio_author).data : NULL;
    qs.tag = FIOBJ_TYPE_IS(fio_tag, FIOBJ_T_STRING) ? fiobj_obj2cstr(fio_tag).data : NULL;
    qs.limit = FIOBJ_TYPE_IS(fio_limit, FIOBJ_T_NUMBER) ? fiobj_obj2num(fio_limit) : NULL;
    qs.offset = FIOBJ_TYPE_IS(fio_offset, FIOBJ_T_NUMBER) ? fiobj_obj2num(fio_offset) : NULL;

    fiobj_free(author_key);
    fiobj_free(tag_key);
    fiobj_free(limit_key);
    fiobj_free(offset_key);

    return qs;
}