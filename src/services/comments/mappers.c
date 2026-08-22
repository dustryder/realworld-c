#include "comments_services.h"
#include "../../data/comment.h"

CommentsServiceResultData map_data_to_comment(
    CommentData *comment_data,
    UserData *user_data,
    bool user_is_following_creator
) {
    CommentsServiceResultData data;

    data.id = comment_data->id;
    data.created_at = comment_data->created_at;
    data.updated_at = comment_data->updated_at;
    data.body = comment_data->body;

    data.author = map_data_to_profile(user_data, user_is_following_creator);

    return data;
}