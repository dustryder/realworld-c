#include "articles_services.h"
#include "../../data/article.h"
#include "../../data/tag.h"
#include "../../lib/mappers.h"

ArticleServiceResult get_article_by_slug(PGconn *conn, int user_id, char* slug) {
    FIO_LOG_DEBUG("get_article_by_slug: user_id=%d, slug=%s", user_id, slug);

    ArticleServiceResult result;
    DataResult article_result = get_article_data_by_slug(conn, slug);
    ArticleData *article_data = article_result.data;

    if (article_result.status == DATA_SUCCESS) {
        DataResult user_result = get_user_data_by_id(conn, article_data->created_by);
        result.status = SERVICE_SUCCESS;
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

        free_ArticleData(article_data);
        free_UserData(user_result.data);
        free(article_data);

        for (int i = 0; i < tag_count; i++) {
            free(tags[i]);
        }
        free(tags);
    } else if (article_result.status == DATA_NOT_FOUND) {
        result.status = SERVICE_NOT_FOUND;
        set_error(&result.error, "article", "not found");
    }

    return result;
}