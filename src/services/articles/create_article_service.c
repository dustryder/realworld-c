#include "articles_services.h"
#include "../../data/article.h"
#include "../../data/tag.h"
#include "../../data/users.h"
#include <ctype.h>
#include "../../lib/string_helpers.h"

ArticleServiceResult create_article(PGconn *conn, int creator, char* title, char* description, char* body, OptionalArray tags) {
    FIO_LOG_DEBUG("create_article: creator=%d, title=%s, description=%s, body=%s", creator, title, description, body);
    int article_count = get_article_count_by_title(conn, title);
    char* slug = sluggify(title, article_count);

    DataResult insert_article_result = insert_article(conn, slug, title, description, body, creator);
    ArticleData *article_data = insert_article_result.data;
    ArticleServiceResult service_result = {0};

    if (insert_article_result.status == DATA_SUCCESS) {

        if (tags.is_present && tags.value != NULL) {
            for (int i = 0; i < tags.value_count; i++) {
                insert_article_tag(conn, article_data->id, tags.value[i]);
            }
        }

        DataResult data_result = get_user_data_by_id(conn, creator);

        service_result.result = map_data_to_article(
            article_data,
            data_result.data,
            tags.value,
            tags.value_count,
            false,
            0,
            false
        );
        service_result.status = SERVICE_SUCCESS;

        free_UserData(data_result.data);
        free_ArticleData(article_data);
        free(article_data);
    }

    free(slug);

    return service_result;
}
