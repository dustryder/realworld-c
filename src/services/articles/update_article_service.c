#include "../../lib/http_helpers.h"
#include "../../data/article.h"
#include "../../data/tag.h"
#include "../../data/users.h"
#include "articles_services.h"
#include "../../lib/type.h"

UpdateArticleResult update_article(
    PGconn *conn,
    char* slug,
    OptionalValue title,
    OptionalValue description,
    OptionalValue body,
    OptionalArray tags
) {
    FIO_LOG_DEBUG("update_article");    
    UpdateValue update_values[3];
    size_t value_count = 0;

    add_field(update_values, &value_count, "title", title);
    add_field(update_values, &value_count, "description", description);
    add_field(update_values, &value_count, "body", body);

    DataResult data_result = update_article_by_slug(conn, slug, update_values, value_count);

    ArticleData *article_data = data_result.data;

    UpdateArticleResult result;

    if (data_result.status == DATA_SUCCESS) {
        if (tags.is_present) {
            delete_article_tags(conn, article_data->id);
            for (int i = 0; i < tags.value_count; i++) {
                insert_article_tag(article_data->id, tags.value[i]);
            }
        }

        DataResult user_result = get_user_data_by_id(conn, article_data->created_by);

        int tag_count;
        char **result_tags = get_tag_by_article_slug(conn, slug, &tag_count);

        result.status = GetArticleSuccess;
        result.result = map_data_to_article(article_data, user_result.data, result_tags, tag_count);
    } else if (data_result.status == DATA_NOT_FOUND) {
        result.status = GET_ARTICLE_UNKNOWN;
    }

    return result;
}