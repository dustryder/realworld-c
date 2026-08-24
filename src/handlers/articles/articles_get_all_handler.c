#include "http.h"
#include "articles_handlers.h"
#include "../../services/articles/articles_services.h"
#include "../../lib/constants.h"

static GetAllArticleQuery parse_GetAllArticleQuery(FIOBJ *params);
static void free_GetAllArticleQuery(GetAllArticleQuery qs);
static void free_GetAllArticleResult(GetAllArticleResult result);

void handle_get_all_articles(http_s* h) {
    FIO_LOG_DEBUG("handle_get_all_articles");

    int id = parse_request_user(h->params);
    GetAllArticleQuery qs = parse_GetAllArticleQuery(h->params);
    char *response_body = NULL;

    GetAllArticleResult result = query_articles(h->udata, qs.author, qs.tag, qs.limit, qs.offset, qs.favorited);

    if (result.status == SERVICE_SUCCESS) {
        response_body = create_many_article_success_response(result.result, result.article_count, result.total_count);
        h->status = HTTP_SUCCESS;
        free_GetAllArticleResult(result);
        free(result.result);
    } else if (result.status == SERVICE_NOT_FOUND) {
        response_body = create_many_article_success_response(result.result, 0, 0);
        h->status = HTTP_SUCCESS;
    } else {
        response_body = create_empty_response();
        h->status = HTTP_INTERNAL_SERVER_ERROR;
    }

    http_send_body(h, response_body, strlen(response_body));

    free(response_body);
    free_GetAllArticleQuery(qs);
}

void free_GetAllArticleResult(GetAllArticleResult result) {
    for (int i = 0; i < result.article_count; i++) {
        free_ArticlesServiceResultData(&result.result[i]);
    }

    // free(result.result);
}

void free_GetAllArticleQuery(GetAllArticleQuery qs) {
    free(qs.author);
    free(qs.favorited);
    free(qs.tag);
}

GetAllArticleQuery parse_GetAllArticleQuery(FIOBJ *params) {

    GetAllArticleQuery qs;

    FIOBJ author_key = fiobj_str_new("author", 6);
    FIOBJ tag_key = fiobj_str_new("tag", 3);
    FIOBJ favorited_key = fiobj_str_new("favorited", 9);
    FIOBJ limit_key = fiobj_str_new("limit", 5);
    FIOBJ offset_key = fiobj_str_new("offset", 6);

    FIOBJ fio_author = fiobj_hash_get(params, author_key);
    FIOBJ fio_tag = fiobj_hash_get(params, tag_key);
    FIOBJ fio_favorited = fiobj_hash_get(params, favorited_key);
    FIOBJ fio_limit = fiobj_hash_get(params, limit_key);
    FIOBJ fio_offset = fiobj_hash_get(params, offset_key);

    qs.author = FIOBJ_TYPE_IS(fio_author, FIOBJ_T_STRING) ? strdup(fiobj_obj2cstr(fio_author).data) : NULL;
    qs.tag = FIOBJ_TYPE_IS(fio_tag, FIOBJ_T_STRING) ? strdup(fiobj_obj2cstr(fio_tag).data) : NULL;
    qs.favorited = FIOBJ_TYPE_IS(fio_favorited, FIOBJ_T_STRING) ? strdup(fiobj_obj2cstr(fio_favorited).data) : NULL;
    qs.limit = FIOBJ_TYPE_IS(fio_limit, FIOBJ_T_NUMBER) ? fiobj_obj2num(fio_limit) : NULL;
    qs.offset = FIOBJ_TYPE_IS(fio_offset, FIOBJ_T_NUMBER) ? fiobj_obj2num(fio_offset) : NULL;

    fiobj_free(author_key);
    fiobj_free(tag_key);
    fiobj_free(favorited_key);
    fiobj_free(limit_key);
    fiobj_free(offset_key);

    return qs;
}