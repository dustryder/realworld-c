#include "comments_services.h"
#include "../../data/article.h"
#include "../../data/users.h"
#include "../../data/comment.h"
#include "../../data/follow.h"

CommentsServiceResult create_comment(PGconn *conn, char *slug, int user_id, char* body) {
    FIO_LOG_DEBUG("create_comment: slug=%s, user_id=%d", slug, user_id);
    CommentsServiceResult service_result;

    DataResult get_article_result = get_article_data_by_slug(conn, slug);
    ArticleData *article_data = get_article_result.data;

    if (get_article_result.status == DATA_SUCCESS) {
    
        DataResult get_user_result = get_user_data_by_id(conn, user_id);
        DataResult insert_comment_result = insert_comment(conn, article_data->id, user_id, body);

        service_result.result = map_data_to_comment(insert_comment_result.data, get_user_result.data, false);
        service_result.status = SUCCESS;
    } else if (get_article_result.status == DATA_NOT_FOUND) {
        service_result.status = NOT_FOUND;
        service_result.error.property = "article";
        service_result.error.error = "not found";
    }

    return service_result;
}
