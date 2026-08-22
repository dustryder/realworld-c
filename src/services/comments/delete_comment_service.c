#include "comments_services.h"
#include "../../data/comment.h"
#include "../../data/article.h"

CommentsServiceResult delete_comment(PGconn *conn, int id, char *slug) {
    FIO_LOG_DEBUG("delete_comment: id=%d", id);
    CommentsServiceResult service_result;

    DataResult get_article_result = get_article_data_by_slug(conn, slug);
    
    if (get_article_result.status == DATA_SUCCESS) {

        DataResult get_comment_result = get_comment_by_id(conn, id);

        printf("%d\n", get_comment_result.status);

        if (get_comment_result.status == DATA_SUCCESS) {
            delete_comment_by_id(conn, id);
            service_result.status = SUCCESS;
        } else if (get_comment_result.status == DATA_NOT_FOUND) {
            service_result.error.property = "comment";
            service_result.error.error = "not found";
            service_result.status = NOT_FOUND;
        }
    } else if (get_article_result.status == DATA_NOT_FOUND) {
        service_result.error.property = "article";
        service_result.error.error = "not found";
        service_result.status = NOT_FOUND;
    }

    return service_result;
}