#include "articles_services.h"
#include "../../data/article.h"

ArticlesServiceResultData map_data_to_article(
    ArticleData *article_data,
    UserData *user_data,
    char** tags,
    int tag_count,
    bool favorited,
    int favoritesCount
) {
    ArticlesServiceResultData data;

    data.slug = article_data->slug;
    data.title = article_data->title;
    data.description = article_data->description;
    data.body = article_data->body;
    data.createdAt = article_data->created_at;
    data.updatedAt = article_data->updated_at;

    data.tagList = tags;
    data.tag_count = tag_count;
    data.favorited = favorited;
    data.favoritesCount = favoritesCount;

    data.author = map_data_to_profile(user_data, false);

    return data;
}