#include "articles_services.h"
#include "../../data/article.h"

GetAllArticleResult get_user_article_feed(PGconn *conn, int user_id, int limit, int offset) {

    GetAllArticleResult result = {0};

    DataResult article_result = get_all_followed_articles(conn, user_id, limit, offset);
    int article_count_result = get_all_followed_articles_count(conn, user_id);

    ArticleDataRecordset *article_data_recordset = article_result.data;

    if (article_result.status == DATA_SUCCESS) {
        ArticlesServiceResultData *result_data = malloc(article_data_recordset->record_count * sizeof *result_data);

        for (int i = 0; i < article_data_recordset->record_count; i++) {

            ArticleData current_record = article_data_recordset->data[i];
            DataResult user_result = get_user_data_by_id(conn, current_record.created_by);

            int tag_count;
            char** tags = get_tag_by_article_slug(conn, current_record.slug, &tag_count);
            int favorite_count = get_article_favorite_count(conn, current_record.slug);
            // int user_follows_article = get_user_follows_article(conn, user_id, slug);

            result_data[i] = map_data_to_article(
                &current_record,
                user_result.data,
                tags,
                tag_count,
                false,
                favorite_count,
                true
            );

            free_ArticleData(&current_record);
            free_UserData(user_result.data);
        }

        result.status = SERVICE_SUCCESS;
        result.result = result_data;
        result.article_count = article_data_recordset->record_count;
        result.total_count = article_count_result;
        free(article_data_recordset->data);
        free(article_data_recordset);

    } else if (article_result.status == DATA_NOT_FOUND) {
        result.status = SERVICE_NOT_FOUND;
        result.article_count = 0;
        result.total_count = 0;
    }

    return result;
}