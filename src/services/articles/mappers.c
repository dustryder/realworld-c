#include "articles_services.h"
#include "../../data/article.h"

ArticlesServiceResultData map_data_to_article(
    ArticleData *article_data,
    UserData *user_data,
    char** tags,
    int tag_count,
    bool favorited,
    int favoritesCount,
    bool user_is_following_creator
) {
    ArticlesServiceResultData data;

    data.slug = strdup(article_data->slug);
    data.title = strdup(article_data->title);
    data.description = strdup(article_data->description);
    data.body = strdup(article_data->body);
    data.createdAt = strdup(article_data->created_at);
    data.updatedAt = strdup(article_data->updated_at);

    char **copied_taglist = NULL;
    if (tag_count > 0) {
        copied_taglist = malloc((sizeof(char *) * tag_count));
    }

    for (int i = 0; i < tag_count; i++) {
        copied_taglist[i] = strdup(tags[i]);
    }

    data.tagList = copied_taglist;
    data.tag_count = tag_count;
    data.favorited = favorited;
    data.favoritesCount = favoritesCount;

    data.author = map_data_to_profile(user_data, user_is_following_creator);

    return data;
}