#include "articles_services.h"
#include "../../data/article.h"
#include "../../data/tag.h"

GetAllArticleResult query_articles(PGconn *conn) {
    FIO_LOG_DEBUG("query_articles");

    GetAllArticleResult result;
    DataResult article_result = get_all_articles(conn);

    ArticleDataRecordset *article_data_recordset = article_result.data;

    ArticlesServiceResultData *result_data = malloc(article_data_recordset->record_count * sizeof *result_data);

    if (article_result.status == DATA_SUCCESS) {

        for (int i = 0; i < article_data_recordset->record_count; i++) {

            ArticleData current_record = article_data_recordset->data[i];
            DataResult user_result = get_user_data_by_id(conn, current_record.created_by);

            int tag_count;
            char** tags = get_tag_by_article_slug(conn, current_record.slug, &tag_count);

            result_data[i] = map_data_to_article(&current_record, user_result.data, tags, tag_count);
        }

        result.status = GetArticleSuccess;
        result.result = result_data;
        result.article_count = article_data_recordset->record_count;

        return result;
    }

    return result;
}