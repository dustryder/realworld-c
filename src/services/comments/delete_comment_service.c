#include "comments_services.h"
#include "../../data/comment.h"
#include "../../data/article.h"
#include "../../lib/mappers.h"

CommentsServiceResult delete_comment(PGconn *conn, int article_id, char *slug, int user_id) {
    FIO_LOG_DEBUG("delete_comment: article_id=%d", article_id);
    CommentsServiceResult service_result;

    DataResult get_article_result = get_article_data_by_slug(conn, slug);
    ArticleData *article_data = get_article_result.data;
    
    if (get_article_result.status == DATA_SUCCESS) {

        DataResult get_comment_result = get_comment_by_id(conn, article_id);
        CommentData *comment_data = get_comment_result.data;

        if (get_comment_result.status == DATA_SUCCESS && comment_data->created_by == user_id) {
            delete_comment_by_id(conn, article_id);
            service_result.status = SERVICE_SUCCESS;
            free_CommentData(comment_data);
            free(comment_data);
        } else if (get_comment_result.status == DATA_NOT_FOUND) {
            service_result.status = SERVICE_NOT_FOUND;
            set_error(&service_result.error, "comment", "not found");
        } else if (comment_data->created_by != user_id) {
            service_result.status = SERVICE_UNAUTHORIZED;
            set_error(&service_result.error, "comment", "forbidden");
            free_CommentData(comment_data);
            free(comment_data);
        }

        free_ArticleData(article_data);
        free(article_data);
    } else if (get_article_result.status == DATA_NOT_FOUND) {
        service_result.status = SERVICE_NOT_FOUND;
        set_error(&service_result.error, "article", "not found");
    }

    return service_result;
}