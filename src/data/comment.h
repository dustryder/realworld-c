#include "../lib/db.h"

#ifndef COMMENT_DATA_H
#define COMMENT_DATA_H
typedef struct {
    int id;
    int article_id;
    int created_by;
    char *body;
    char *created_at;
    char *updated_at;
} CommentData;

typedef struct {
    CommentData *data;
    int record_count;
} CommentDataRecordset;

DataResult insert_comment(PGconn *conn, int article_id, int created_by, char *body);
DataResult get_all_comments_by_article_id(PGconn *conn, int article_id);
DataResult get_comment_by_id(PGconn *conn, int id);
DataResult delete_comment_by_id(PGconn *conn, int id);
#endif