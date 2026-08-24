#include "articles_services.h"
#include "../../data/article.h"
#include "../../data/tag.h"

GetAllArticleResult query_articles(PGconn *conn, char *author, char *tag, int limit, int offset, char *favorited) {
    FIO_LOG_DEBUG("query_articles: author=%s, tag=%s, limit=%d, offset=%d", author, tag, limit, offset);

    GetAllArticleResult result = {0};
    DataResult article_result = get_all_articles(conn, author, tag, limit, offset, favorited);
    int article_count_result = get_all_articles_count(conn);

    ArticleDataRecordset *article_data_recordset = article_result.data;

    printf("Article status: %d\n", article_result.status);

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
                false
            );
            free_ArticleData(&current_record);
            free_UserData(user_result.data);

            for (int i = 0; i < tag_count; i++) {
                free(tags[i]);
            }

            free(tags);
        }

        result.status = SERVICE_SUCCESS;
        result.result = result_data;
        result.article_count = article_data_recordset->record_count;
        result.total_count = article_count_result;
        free(article_data_recordset->data);
        free(article_data_recordset);

        return result;
    } else if (article_result.status == DATA_NOT_FOUND) {
        result.status = SERVICE_NOT_FOUND;
        result.article_count = 0;
    }

    return result;
}