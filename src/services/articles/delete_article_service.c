#include "articles_services.h"
#include "../../data/article.h"
#include "../../data/tag.h"

void delete_article(
    PGconn *conn,
    char* slug
) {
    FIO_LOG_DEBUG("delete_article: slug=%s", slug);   
    
    DataResult article_result = get_article_data_by_slug(conn, slug);
    ArticleData *article_data = article_result.data;

    delete_article_tags(conn, article_data->id);
    delete_article_by_id(conn, article_data->id);

    return;
}