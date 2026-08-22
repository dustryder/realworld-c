#include "../../lib/type.h"
#include "../profiles/profiles_services.h"
#include "../../data/comment.h"

#ifndef COMMENTS_SERVICES_H
#define COMMENTS_SERVICES_H
typedef struct {
    int id;
    char *created_at;
    char *updated_at;
    char *body;
    ProfileServiceResultData author;
} CommentsServiceResultData;

typedef struct {
    ServiceStatus status;
    CommentsServiceResultData result;
    ErrorValue error;
} CommentsServiceResult;

typedef struct {
    ServiceStatus status;
    CommentsServiceResultData *result;
    int result_count;
    ErrorValue error;
} AllCommentsServiceResult;

CommentsServiceResult create_comment(PGconn *conn, char *slug, int user_id, char *body);
CommentsServiceResult delete_comment(PGconn *conn, int id, char *slug);
AllCommentsServiceResult get_article_comments(PGconn *conn, char *slug);

CommentsServiceResultData map_data_to_comment(
    CommentData *comment_data,
    UserData *user_data,
    bool user_is_following_creator
);
#endif