#include "comments_services.h"
#include "../../data/comment.h"
#include "../../data/article.h"
#include "../../lib/mappers.h"

AllCommentsServiceResult get_article_comments(PGconn *conn, char *slug) {
    FIO_LOG_DEBUG("get_article_comments: slug=%s", slug);
    AllCommentsServiceResult service_result;

    DataResult get_article_result = get_article_data_by_slug(conn, slug);
    ArticleData *article_data = get_article_result.data;

    if (get_article_result.status == DATA_SUCCESS) {
        DataResult get_article_comments_result = get_all_comments_by_article_id(conn, article_data->id);
        CommentDataRecordset *comments_recordset = get_article_comments_result.data;
    
        CommentsServiceResultData *result_data = malloc(comments_recordset->record_count * sizeof *result_data);

        for (int i = 0; i < comments_recordset->record_count; i++) {
            DataResult get_user_result = get_user_data_by_id(conn, comments_recordset->data[i].created_by);

            result_data[i] = map_data_to_comment(&comments_recordset->data[i], get_user_result.data, false);
        }

        service_result.result = result_data;
        service_result.result_count = comments_recordset->record_count;
        service_result.status = SERVICE_SUCCESS;
    } else if (get_article_result.status == DATA_NOT_FOUND) {
        service_result.status = SERVICE_NOT_FOUND;
        set_error(service_result.error, "article", "not found");
    }

    return service_result;
}