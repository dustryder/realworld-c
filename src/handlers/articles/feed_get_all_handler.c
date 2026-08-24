#include "main.h"
#include "articles_handlers.h"
#include "../../lib/constants.h"

static GetAllFeedQuery parse_GetAllFeedQuery(FIOBJ *params);

void handle_get_all_feed(http_s* h) {
    FIO_LOG_DEBUG("handle_get_all_feed");

    int id = parse_request_user(h->params);
    GetAllFeedQuery qs = parse_GetAllFeedQuery(h->params);

    char *response_body = NULL;

    GetAllArticleResult result = get_user_article_feed(h->udata, id, qs.limit, qs.offset);

    if (result.status == SERVICE_SUCCESS || result.status == SERVICE_NOT_FOUND) {
        response_body = create_many_article_success_response(result.result, result.article_count, result.total_count);
        h->status = HTTP_SUCCESS;

        if (result.status == SERVICE_SUCCESS) {
            for (int i = 0; i < result.article_count; i++) {
                free_ArticlesServiceResultData(&result.result[i]);
            }
            free(result.result);
        }
    } 
    else {
        response_body = create_empty_response();
        h->status = HTTP_INTERNAL_SERVER_ERROR;
    }

    http_send_body(h, response_body, strlen(response_body));

    free(response_body);
}

GetAllFeedQuery parse_GetAllFeedQuery(FIOBJ *params) {

    GetAllFeedQuery qs;

    FIOBJ limit_key = fiobj_str_new("limit", 5);
    FIOBJ offset_key = fiobj_str_new("offset", 6);

    FIOBJ fio_limit = fiobj_hash_get(params, limit_key);
    FIOBJ fio_offset = fiobj_hash_get(params, offset_key);

    qs.limit = FIOBJ_TYPE_IS(fio_limit, FIOBJ_T_NUMBER) ? fiobj_obj2num(fio_limit) : NULL;
    qs.offset = FIOBJ_TYPE_IS(fio_offset, FIOBJ_T_NUMBER) ? fiobj_obj2num(fio_offset) : NULL;

    fiobj_free(limit_key);
    fiobj_free(offset_key);

    return qs;
}