#include "article.h"
#include "../lib/http_helpers.h"
#include <libpq-fe.h>
#include "../lib/db.h"

static ArticleData *map_article_data(const PGresult *res);
static ArticleDataRecordset *map_many_article_data(const PGresult *res);

DataResult insert_article(char* slug, char* title, char* description, char *body, int created_by) {
    FIO_LOG_DEBUG("insert_article: slug: %s, title: %s, description: %s created_by: %d", slug, title, description, created_by);
    PGconn *connection = get_connection();
    char created_by_str[20];
    sprintf(created_by_str, "%d", created_by);

    char* command = "INSERT INTO \"article\" (slug, title, description, body, created_by) VALUES"
                    "($1, $2, $3, $4, $5)"
                    "RETURNING *";
    const char * const data[5] = { slug, title, description, body, created_by_str};

    PGresult *data_result = PQexecParams(connection,command,5,NULL,data,NULL,NULL,0);

    DataResult result = get_data_result(data_result, map_article_data);

    return result;
}

void delete_article_by_id(PGconn *conn, int id) {
    FIO_LOG_DEBUG("delete_article_by_id: id=%d", id);
    char article_id[20];
    sprintf(article_id, "%d", id);

    char* command = "DELETE FROM \"article\""
                    "WHERE id = $1";

    const char * const data[1] = { article_id };

    PGresult *data_result = PQexecParams(conn,command,1,NULL,data,NULL,NULL,0);

    return;
}

DataResult get_article_data_by_slug(char* slug) {
    FIO_LOG_DEBUG("insert_article: slug: %s", slug);
    PGconn *connection = get_connection();

    char* command = "SELECT * FROM \"article\""
                    "WHERE slug = $1";
    const char * const data[1] = { slug};

    PGresult *data_result = PQexecParams(connection,command,1,NULL,data,NULL,NULL,0);

    DataResult result = get_data_result(data_result, map_article_data);

    return result;
}

DataResult get_all_articles(PGconn *conn, char *author, char *tag) {
    FIO_LOG_DEBUG("get_all_articles: author=%s, tag=%s", author, tag);
    
    char *BASE_AUTHOR_QUERY = "created_by = (SELECT id FROM \"user\" WHERE username = $%d)";
    char *BASE_TAG_QUERY = "id IN (SELECT article_id FROM \"tag\" JOIN article_tag ON article_tag.tag_id = tag.id WHERE name = $%d)";
    char *BASE_QUERY = "SELECT * FROM \"article\"";

    int max_command_length = strlen(BASE_AUTHOR_QUERY) + strlen(BASE_TAG_QUERY) + strlen(BASE_QUERY) + 500;
    char *data[2];
    int data_count = 0;

    char filters[max_command_length];
    memset(filters, '\0', max_command_length);
    char command[max_command_length];
    memset(command, '\0', max_command_length);

    sprintf(command, BASE_QUERY);

    if (author != NULL) {

        if (data_count == 0) {
            sprintf(command + strlen(command), "%s", " WHERE");
        }

        if (data_count > 0) {
            sprintf(command + strlen(command), " %s ", "AND");
        }

        char author_buffer[strlen(BASE_AUTHOR_QUERY) + strlen(author) + 500];
        memset(author_buffer, '\0', strlen(BASE_AUTHOR_QUERY) + strlen(author) + 1);

        sprintf(author_buffer, BASE_AUTHOR_QUERY, data_count + 1);
        sprintf(command + strlen(command), " %s", author_buffer);

        data[data_count] = author;
        data_count += 1;
    }

    if (tag != NULL) {

        if (data_count == 0) {
            sprintf(command + strlen(command), "%s", " WHERE");
        }

        if (data_count > 0) {
            sprintf(command + strlen(command), " %s ", "AND");
        }

        char tag_buffer[strlen(BASE_TAG_QUERY) + strlen(tag) + 500];
        memset(tag_buffer, '\0', strlen(BASE_TAG_QUERY) + strlen(tag) + 1);

        sprintf(tag_buffer, BASE_TAG_QUERY, data_count + 1);
        sprintf(command + strlen(command), " %s", tag_buffer);

        data[data_count] = tag;
        data_count += 1;
    }

    PGresult *data_result = PQexecParams(conn,command,data_count,NULL,data,NULL,NULL,0);

    DataResult result = get_data_result(data_result, map_many_article_data);

    ArticleDataRecordset *typ = result.data;

    return result;
}

int get_article_count_by_title(char* title) {
    FIO_LOG_DEBUG("get_article_by_slug: title: %s", title);
    PGconn *connection = get_connection();

    char* command = "SELECT COUNT(*) FROM \"article\""
                    "WHERE title = $1";
    const char * const data[1] = { title, };

    PGresult *data_result = PQexecParams(connection,command,1,NULL,data,NULL,NULL,0);

    int result = get_article_count_result(data_result);

    return result;
}

int get_article_count_result(const PGresult *res) {
    ExecStatusType command_status = PQresultStatus(res);

    if (command_status == PGRES_TUPLES_OK) {
        return strtol(PQgetvalue(res, 0, 0), NULL, 10);
    }

    return 0;
}

DataResult update_article_by_slug(PGconn *conn, char *slug, UpdateValue *update_values, size_t update_count) {
    FIO_LOG_DEBUG("update_article_by_slug");

    char *data[update_count + 1];

    char* update_updated_at = "updated_at = (now() at time zone 'utc')";

    int update_substring_length = strlen(update_updated_at) + 3;

    for (int i = 0; i < update_count; i++) {
        update_substring_length += strlen(update_values[i].value);
    }
    
    char update_substring[update_substring_length];
    memset(update_substring, 0, sizeof(update_substring));

    char *base_command = "UPDATE \"article\" "
                         "SET %s "
                         "WHERE slug = $1 "
                         "RETURNING *";

    data[0] = slug;

    for (int i = 1; i < update_count + 1; i++) {
        data[i] = update_values[i - 1].value;
    }

    for (int i = 0; i < update_count; i++) {
        sprintf(update_substring + strlen(update_substring), "%s = $%d", update_values[i].key, i + 2);

        if (i != update_count - 1) {
            sprintf(update_substring + strlen(update_substring), ",");
        }
    };

    if (update_count > 0) {
        sprintf(update_substring + strlen(update_substring), ", %s", update_updated_at);
    } else {
        sprintf(update_substring + strlen(update_substring), "%s", update_updated_at);
    }

    char command[strlen(base_command) + strlen(update_substring)];
    sprintf(command, base_command, update_substring);

    PGresult *data_result = PQexecParams(conn, command, update_count + 1, NULL, data, NULL, NULL, 0);

    DataResult result = get_data_result(data_result, map_article_data);

    return result;
}

ArticleDataRecordset *map_many_article_data(const PGresult *res) {

    int row_count = PQntuples(res);

    ArticleDataRecordset *recordset = malloc(sizeof *recordset);
    recordset->record_count = row_count;
    recordset->data = malloc(row_count * sizeof(*recordset->data));

    for (int i = 0; i < row_count; i++) {
        recordset->data[i].id = strtol(PQgetvalue(res, i, 0), NULL, 10);
        recordset->data[i].slug = PQgetvalue(res, i, 1);
        recordset->data[i].title = PQgetvalue(res, i, 2);
        recordset->data[i].body = PQgetvalue(res, i, 3);
        recordset->data[i].description = PQgetvalue(res, i, 4);
        recordset->data[i].created_at = PQgetvalue(res, i, 5);
        recordset->data[i].updated_at = PQgetvalue(res, i, 6);
        recordset->data[i].created_by = strtol(PQgetvalue(res, i, 7), NULL, 10);
    }

    return recordset;
}

ArticleData *map_article_data(const PGresult *res) {
    ArticleData *data = malloc(sizeof *data);

    data->id = strtol(PQgetvalue(res, 0, 0), NULL, 10);
    data->slug = PQgetvalue(res, 0, 1);
    data->title = PQgetvalue(res, 0, 2);
    data->body = PQgetvalue(res, 0, 3);
    data->description = PQgetvalue(res, 0, 4);
    data->created_at = PQgetvalue(res, 0, 5);
    data->updated_at = PQgetvalue(res, 0, 6);
    data->created_by = strtol(PQgetvalue(res, 0, 7), NULL, 10);

    return data;
}
