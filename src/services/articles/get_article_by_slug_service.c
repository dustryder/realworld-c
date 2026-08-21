#include "articles_services.h"
#include "../../data/article.h"
#include "../../data/tag.h"

GetArticleResult get_article_by_slug(PGconn *conn, int user_id, char* slug) {
    FIO_LOG_DEBUG("get_article_by_slug: user_id=%d, slug=%s", user_id, slug);

    GetArticleResult result;
    DataResult article_result = get_article_data_by_slug(slug);
    ArticleData *article_data = article_result.data;

    if (article_result.status == DATA_SUCCESS) {
        DataResult user_result = get_user_data_by_id(conn, article_data->created_by);
        result.status = GetArticleSuccess;
        int tag_count;
        char **tags = get_tag_by_article_slug(conn, slug, &tag_count);

        result.result = map_data_to_article(article_result.data, user_result.data, tags, tag_count);
    } else if (article_result.status == DATA_NOT_FOUND) {
        result.status = GET_ARTICLE_UNKNOWN;
    }

    return result;
}