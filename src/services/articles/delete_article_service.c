#include "articles_services.h"
#include "../../data/article.h"
#include "../../data/tag.h"
#include "../../lib/mappers.h"

ArticleServiceResult delete_article(
    PGconn *conn,
    char* slug,
    int user_id
) {
    FIO_LOG_DEBUG("delete_article: slug=%s, user_id=%d", slug, user_id);   
    
    DataResult article_result = get_article_data_by_slug(conn, slug);
    ArticleData *article_data = article_result.data;
    ArticleServiceResult result;

    if (article_result.status == DATA_NOT_FOUND) {
        result.status = SERVICE_NOT_FOUND;
        set_error(&result.error, "article", "not found");
        return result;
    } else if (article_data->created_by != user_id) {
        result.status = SERVICE_UNAUTHORIZED;
        set_error(&result.error, "article", "forbidden");
        return result;
    }

    result.status = SERVICE_SUCCESS;
    delete_article_by_id(conn, article_data->id);

    return result;
}