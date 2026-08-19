#include "articles_services.h"
#include "../../data/article.h"

GetArticleResult get_article_by_slug(int user_id, char* slug) {
    FIO_LOG_DEBUG("get_article_by_slug: user_id=%d, slug=%s", user_id, slug);

    GetArticleResult result;
    DataResult article_result = get_article_data_by_slug(slug);
    DataResult user_result = get_user_data_by_id(user_id);

    if (article_result.status == DATA_SUCCESS) {
        result.status = GetArticleSuccess;
        int tag_count;
        char **tags = get_tag_by_article_slug(slug, &tag_count);
        result.result = map_data_to_article(article_result.data, user_result.data, tags, tag_count);

        return result;
    }
}