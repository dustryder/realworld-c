#include "main.h"
#include "articles_handlers.h"
#include "../../lib/constants.h"

static GetAllFeedQuery parse_get_all_feed_qs(FIOBJ *params);

void handle_get_all_feed(http_s* h) {
    FIO_LOG_DEBUG("handle_get_all_feed");

    int id = parse_request_user(h->params);
    GetAllFeedQuery qs = parse_get_all_feed_qs(h->params);

    char *response_body;

    GetAllArticleResult result = get_user_article_feed(h->udata, id, qs.limit, qs.offset);

    response_body = create_many_article_success_response(result.result, result.article_count, result.total_count);
    h->status = HTTP_SUCCESS;

    http_send_body(h, response_body, strlen(response_body));
}

GetAllFeedQuery parse_get_all_feed_qs(FIOBJ *params) {

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