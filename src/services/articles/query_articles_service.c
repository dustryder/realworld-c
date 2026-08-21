#include "articles_services.h"
#include "../../data/article.h"
#include "../../data/tag.h"

GetAllArticleResult query_articles(PGconn *conn, char *author, char *tag, int limit, int offset) {
    FIO_LOG_DEBUG("query_articles: author=%s, tag=%s, limit=%d, offset=%d", author, tag, limit, offset);

    GetAllArticleResult result;
    DataResult article_result = get_all_articles(conn, author, tag, limit, offset);
    int article_count_result = get_all_articles_count(conn);

    ArticleDataRecordset *article_data_recordset = article_result.data;

    if (article_result.status == DATA_SUCCESS) {
        printf("Success?\n");
        printf("%d\n", article_data_recordset->record_count);
        ArticlesServiceResultData *result_data = malloc(article_data_recordset->record_count * sizeof *result_data);

        for (int i = 0; i < article_data_recordset->record_count; i++) {

            ArticleData current_record = article_data_recordset->data[i];
            DataResult user_result = get_user_data_by_id(conn, current_record.created_by);

            int tag_count;
            char** tags = get_tag_by_article_slug(conn, current_record.slug, &tag_count);

            printf("Mapping\n");
            result_data[i] = map_data_to_article(&current_record, user_result.data, tags, tag_count);
        }

        result.status = GetArticleSuccess;
        result.result = result_data;
        result.article_count = article_data_recordset->record_count;
        result.total_count = article_count_result;

        printf("article count service: %d\n", result.article_count);

        return result;
    } else if (article_result.status == DATA_NOT_FOUND) {
        result.status = GET_ARTICLE_UNKNOWN;
        result.article_count = 0;
    }

    return result;
}