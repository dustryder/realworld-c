#include "articles_services.h"
#include "../../data/article.h"
#include "../../data/tag.h"
#include "../../data/users.h"
#include <ctype.h>

static char* sluggify(char* title, int title_count);

ArticleServiceResult create_article(PGconn *conn, int creator, char* title, char* description, char* body, OptionalArray tags) {
    FIO_LOG_DEBUG("create_article: creator=%d, title=%s, description=%s, body=%s", creator, title, description, body);
    int article_count = get_article_count_by_title(conn, title);
    char* slug = sluggify(title, article_count);

    DataResult insert_article_result = insert_article(conn, slug, title, description, body, creator);
    ArticleData *article_data = insert_article_result.data;
    ArticleServiceResult service_result;

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

char* sluggify(char* title, int title_count) {
    FIO_LOG_DEBUG("sluggify: title=%s, title_count=%d", title, title_count);
    int suffix_length = snprintf(NULL, 0, "-%d", title_count);
    char* result_string = malloc((strlen(title) * sizeof(char)) + suffix_length + 1);
    int result_string_pointer = 0;

    size_t title_length = strlen(title);

    for (size_t i = 0; i < title_length; i++) {
        char current_character = title[i];

        if (isalnum(current_character)) {
            result_string[result_string_pointer] = tolower(current_character);
            result_string_pointer += 1;
        } else if (current_character == ' ') {
            result_string[result_string_pointer] = '-';
            result_string_pointer += 1;
        }
    }

    result_string[result_string_pointer] = '\0';

    sprintf(result_string + strlen(result_string), "-%d", title_count);

    return result_string;
}
