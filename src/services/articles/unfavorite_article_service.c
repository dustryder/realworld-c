#include "articles_services.h"
#include "../../data/article.h"

ArticleServiceResult unfavorite_article(PGconn *conn, int user_id, char *slug) {
    FIO_LOG_DEBUG("unfavorite_article: user_id=%d, slug=%s", user_id, slug);

    ArticleServiceResult result;
    DataResult article_result = get_article_data_by_slug(conn, slug);
    ArticleData *article_data = article_result.data;

    if (article_result.status == DATA_NOT_FOUND) {
        result.status = GET_ARTICLE_UNKNOWN;
        result.error.property = "resource";
        result.error.error = "not found";
        return result;
    }

    if (article_result.status == DATA_SUCCESS) {
        delete_article_favorite(conn, user_id, slug);
        DataResult user_result = get_user_data_by_id(conn, article_data->created_by);

        result.status = GetArticleSuccess;
        int tag_count;
        char **tags = get_tag_by_article_slug(conn, slug, &tag_count);

        int favorite_count = get_article_favorite_count(conn, slug);
        int user_favorites_article = get_user_favorites_article(conn, user_id, slug);

        result.result = map_data_to_article(
            article_result.data,
            user_result.data,
            tags,
            tag_count,
            user_favorites_article,
            favorite_count,
            false
        );
    } else if (article_result.status == DATA_NOT_FOUND) {
        result.status = GET_ARTICLE_UNKNOWN;
        result.error.property = "article";
        result.error.error = "not found";
    }

    return result;
}