#include "articles_services.h"
#include "articles_handlers.h"
#include "cJSON.h"
#include "../../lib/string_helpers.h"

static cJSON *create_article_json(ArticlesServiceResultData result, bool include_body, DateTimeFormat format);

char *create_many_article_success_response(ArticlesServiceResultData *results, int result_count, int total_count) {
    cJSON *response_body = cJSON_CreateObject();
    cJSON *articles_body = cJSON_CreateArray();
    
    for (int i = 0; i < result_count; i++) {
        cJSON *article_body = create_article_json(results[i], false, FORMAT_DATETIMESTAMP);
        cJSON_AddItemToArray(articles_body, article_body);
    }

    cJSON_AddItemToObject(response_body, "articles", articles_body);
    cJSON_AddItemToObject(response_body, "articlesCount", cJSON_CreateNumber(total_count));

    char *response_string = cJSON_Print(response_body);
    cJSON_Delete(response_body);

    return response_string;
}

char* create_article_success_response(ArticlesServiceResultData result, bool include_body, DateTimeFormat format) {
    cJSON *response_body = cJSON_CreateObject();
    
    cJSON *article_body = create_article_json(result, include_body, format);

    cJSON_AddItemToObject(response_body, "article", article_body);

    char *response_string = cJSON_Print(response_body);
    cJSON_Delete(response_body);

    return response_string;
}

static cJSON *create_article_json(ArticlesServiceResultData result, bool include_body, DateTimeFormat format) {
    cJSON *article_properties = cJSON_CreateObject();
    cJSON *author_properties = cJSON_CreateObject();

    char *created_at = format == FORMAT_DATESTAMP ? datetimestamp_to_date(result.createdAt) : datetimestamp_to_datetimestamp(result.createdAt);
    char *updated_at = format == FORMAT_DATESTAMP ? datetimestamp_to_date(result.updatedAt) : datetimestamp_to_datetimestamp(result.updatedAt);

    cJSON_AddItemToObject(author_properties, "username", cJSON_CreateString(result.author.username));
    cJSON_AddItemToObject(author_properties, "bio", cJSON_CreateString(result.author.bio));
    cJSON_AddItemToObject(author_properties, "image", cJSON_CreateString(result.author.image));
    cJSON_AddItemToObject(author_properties, "following", cJSON_CreateBool(result.author.following));

    cJSON_AddItemToObject(article_properties, "slug", cJSON_CreateString(result.slug));
    cJSON_AddItemToObject(article_properties, "title", cJSON_CreateString(result.title));
    cJSON_AddItemToObject(article_properties, "description", cJSON_CreateString(result.description));
    if (include_body) cJSON_AddItemToObject(article_properties, "body", cJSON_CreateString(result.body));

    if (result.tagList != NULL) {
        cJSON_AddItemToObject(article_properties, "tagList", cJSON_CreateStringArray((const char *const *)result.tagList, result.tag_count));
    } else {
        cJSON_AddItemToObject(article_properties, "tagList", cJSON_CreateArray());
    }
    cJSON_AddItemToObject(article_properties, "createdAt", cJSON_CreateString(created_at));
    cJSON_AddItemToObject(article_properties, "updatedAt", cJSON_CreateString(updated_at));
    cJSON_AddItemToObject(article_properties, "favorited", cJSON_CreateBool(result.favorited));
    cJSON_AddItemToObject(article_properties, "favoritesCount", cJSON_CreateNumber(result.favoritesCount));

    cJSON_AddItemToObject(article_properties, "author", author_properties);

    return article_properties;
}