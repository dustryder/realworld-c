#include "comments_services.h"
#include "../../data/comment.h"
#include "../../data/article.h"
#include "../../lib/mappers.h"
#include "../../data/follow.h"

AllCommentsServiceResult get_article_comments(PGconn *conn, char *slug, int user_id) {
    FIO_LOG_DEBUG("get_article_comments: slug=%s", slug);
    AllCommentsServiceResult service_result = {0};

    DataResult get_article_result = get_article_data_by_slug(conn, slug);
    ArticleData *article_data = get_article_result.data;

    if (get_article_result.status == DATA_SUCCESS) {
        DataResult get_article_comments_result = get_all_comments_by_article_id(conn, article_data->id);

        if (get_article_comments_result.status == DATA_SUCCESS) {
            CommentDataRecordset *comments_recordset = get_article_comments_result.data;
    
            CommentsServiceResultData *result_data = malloc(comments_recordset->record_count * sizeof *result_data);

            for (int i = 0; i < comments_recordset->record_count; i++) {
                DataResult get_user_result = get_user_data_by_id(conn, comments_recordset->data[i].created_by);
                UserData *user_data = get_user_result.data;

                int user_follows_comment_creator = user_id != -1 ? get_user_follows_user(conn, user_id, comments_recordset->data[i].created_by) : 0;

                result_data[i] = map_data_to_comment(&comments_recordset->data[i], user_data, user_follows_comment_creator);

                free_UserData(user_data);
                free_CommentData(&comments_recordset->data[i]);
            }

            service_result.result = result_data;
            service_result.result_count = comments_recordset->record_count;
            free(comments_recordset->data);
            free(comments_recordset);
        } else {
            service_result.result_count = 0;
        }
        service_result.status = SERVICE_SUCCESS;
        free_ArticleData(article_data);
        free(article_data);
    } else if (get_article_result.status == DATA_NOT_FOUND) {
        service_result.status = SERVICE_NOT_FOUND;
        set_error(&service_result.error, "article", "not found");
    }

    return service_result;
}