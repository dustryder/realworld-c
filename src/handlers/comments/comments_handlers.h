#include "main.h"
#include "../../services/comments/comments_services.h"

#ifndef COMMENTS_HANDLERS_H
#define COMMENTS_HANDLERS_H

typedef struct {
    char* body;
} PostCommentPayload;

void handle_post_comments(http_s *h);
void handle_delete_comments(http_s *h);
void handle_get_comments(http_s *h);

char *create_comment_success_response(CommentsServiceResultData result);
char *create_many_comment_success_response(CommentsServiceResultData *result, int result_count);
#endif