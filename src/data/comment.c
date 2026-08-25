#include "comment.h"
#include <libpq-fe.h>
#include "../lib/string_helpers.h"

static CommentData *map_comment_data(const PGresult *res);
static CommentDataRecordset *map_many_comment_data(const PGresult *res);

DataResult insert_comment(PGconn *conn, int article_id, int created_by, char *body) {
    FIO_LOG_DEBUG("insert_comment: article_id=%d, created_by=%d, body=%s", article_id, created_by, body);
    char *command = "INSERT INTO comment (article_id, created_by, body) VALUES "
                    "($1, $2, $3)"
                    "RETURNING *";

    char *created_by_str = number_to_string(created_by);
    char *article_id_str = number_to_string(article_id);

    const char * const data[3] = { article_id_str, created_by_str, body };

    PGresult *data_result = PQexecParams(conn, command, 3, NULL, data, NULL, NULL, 0);

    DataResult result = get_data_result(data_result, (Mapper)map_comment_data);

    PQclear(data_result);
    free(created_by_str);
    free(article_id_str);
    return result;
}

DataResult get_all_comments_by_article_id(PGconn *conn, int article_id) {
    FIO_LOG_DEBUG("get_all_comments_by_article_id: article_id=%d", article_id);
    char *command = "SELECT * FROM comment WHERE article_id = $1";

    char *article_id_str = number_to_string(article_id);

    const char * const data[1] = { article_id_str };

    PGresult *data_result = PQexecParams(conn, command, 1, NULL, data, NULL, NULL, 0);

    DataResult result = get_data_result(data_result, (Mapper)map_many_comment_data);

    PQclear(data_result);
    free(article_id_str);
    return result;
}

DataResult get_comment_by_id(PGconn *conn, int id) {
    FIO_LOG_DEBUG("get_comment_by_id: id=%d", id);
    char *command = "SELECT * FROM comment WHERE id = $1";

    char *id_str = number_to_string(id);
    const char * const data[1] = { id_str };

    PGresult *data_result = PQexecParams(conn, command, 1, NULL, data, NULL, NULL, 0);

    DataResult result = get_data_result(data_result, (Mapper)map_comment_data);

    PQclear(data_result);
    free(id_str);
    return result;
}

DataResult delete_comment_by_id(PGconn *conn, int id) {
    FIO_LOG_DEBUG("delete_comment_by_id: id=%d", id);
    char *command = "DELETE FROM comment WHERE id = $1";

    char *id_str = number_to_string(id);

    const char * const data[1] = { id_str };

    PGresult *data_result = PQexecParams(conn, command, 1, NULL, data, NULL, NULL, 0);
    DataResult result = get_data_result(data_result, NULL);

    PQclear(data_result);
    free(id_str);
    return result;
}

void free_CommentData(CommentData *data) {
    free(data->body);
    free(data->created_at);
    free(data->updated_at);
}

CommentData *map_comment_data(const PGresult *res) {
    CommentData *data = malloc(sizeof *data);

    data->id = strtol(PQgetvalue(res, 0, 0), NULL, 10);
    data->article_id = strtol(PQgetvalue(res, 0, 1), NULL, 10);
    data->created_by = strtol(PQgetvalue(res, 0, 2), NULL, 10);
    data->body = strdup(PQgetvalue(res, 0, 3));
    data->created_at = strdup(PQgetvalue(res, 0, 4));
    data->updated_at = strdup(PQgetvalue(res, 0, 5));

    return data;
}

CommentDataRecordset *map_many_comment_data(const PGresult *res) {

    int row_count = PQntuples(res);

    CommentDataRecordset *recordset = malloc(sizeof *recordset);
    recordset->record_count = row_count;
    recordset->data = malloc(row_count * sizeof(*recordset->data));

    for (int i = 0; i < row_count; i++) {
        recordset->data[i].id = strtol(PQgetvalue(res, i, 0), NULL, 10);
        recordset->data[i].article_id = strtol(PQgetvalue(res, i, 1), NULL, 10);
        recordset->data[i].created_by = strtol(PQgetvalue(res, i, 2), NULL, 10);
        recordset->data[i].body = strdup(PQgetvalue(res, i, 3));
        recordset->data[i].created_at = strdup(PQgetvalue(res, i, 4));
        recordset->data[i].updated_at = strdup(PQgetvalue(res, i, 5));
    }

    return recordset;
}