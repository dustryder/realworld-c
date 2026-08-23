#include "article.h"
#include "../lib/http_helpers.h"
#include <libpq-fe.h>
#include "../lib/db.h"

static ArticleData *map_article_data(const PGresult *res);
static ArticleDataRecordset *map_many_article_data(const PGresult *res);

DataResult insert_article(PGconn *conn, char* slug, char* title, char* description, char *body, int created_by) {
    FIO_LOG_DEBUG("insert_article: slug: %s, title: %s, description: %s created_by: %d", slug, title, description, created_by);
    char created_by_str[20];
    sprintf(created_by_str, "%d", created_by);

    char* command = "INSERT INTO \"article\" (slug, title, description, body, created_by) VALUES "
                    "($1, $2, $3, $4, $5)"
                    "RETURNING *";
    const char * const data[5] = { slug, title, description, body, created_by_str};

    PGresult *data_result = PQexecParams(conn,command,5,NULL,data,NULL,NULL,0);

    DataResult result = get_data_result(data_result, map_article_data);

    PQclear(data_result);
    return result;
}

void insert_article_favorite(PGconn *conn, int user_id, char* slug) {
    FIO_LOG_DEBUG("insert_article_favorite: user_id=%d, slug=%s", user_id, slug);
    char user_id_str[20];
    sprintf(user_id_str, "%d", user_id);

    char *command = "INSERT INTO article_favourite (article_id, user_id) VALUES "
                    "((SELECT id FROM article WHERE slug = $1), $2)";
    
    const char * const data[2] = { slug, user_id_str };
    PGresult *data_result = PQexecParams(conn,command,2,NULL,data,NULL,NULL,0);

    PQclear(data_result);
    return;
}

int get_article_favorite_count(PGconn *conn, char* slug) {
    FIO_LOG_DEBUG("get_article_favorite_count: slug=%s", slug);

    char *command = "SELECT COUNT(*) FROM article_favourite "
                    "WHERE article_id = (SELECT id FROM article WHERE slug = $1)";

    const char * const data[1] = { slug };
    PGresult *data_result = PQexecParams(conn,command,1,NULL,data,NULL,NULL,0);

    int result = get_article_count_result(data_result);

    PQclear(data_result);
    return result;
}

void delete_article_favorite(PGconn *conn, int user_id, char* slug) {
    FIO_LOG_DEBUG("delete_article_favorite: user_id=%d, slug=%s", user_id, slug);
    char user_id_str[20];
    sprintf(user_id_str, "%d", user_id);

    char *command = "DELETE FROM article_favourite "
                    "WHERE article_id = (SELECT id FROM article WHERE slug = $1) AND "
                    "user_id = $2";
    
    const char * const data[2] = { slug, user_id_str };
    PGresult *data_result = PQexecParams(conn,command,2,NULL,data,NULL,NULL,0);

    PQclear(data_result);
    return;
}

int get_user_favorites_article(PGconn *conn, int user_id, char* slug) {
    FIO_LOG_DEBUG("get_user_favorites_article: user_id=%d, slug=%s", user_id, slug);
    char user_id_str[20];
    sprintf(user_id_str, "%d", user_id);

    char *command = "SELECT 1 FROM article_favourite "
                    "WHERE user_id = $1 AND article_id = (SELECT id FROM article WHERE slug = $2)";

    const char * const data[2] = { user_id_str, slug };

    PGresult *data_result = PQexecParams(conn,command,2,NULL,data,NULL,NULL,0);

    int result = get_article_count_result(data_result);

    PQclear(data_result);
    return result;
}

void delete_article_by_id(PGconn *conn, int id) {
    FIO_LOG_DEBUG("delete_article_by_id: id=%d", id);
    char article_id[20];
    sprintf(article_id, "%d", id);

    char* command = "DELETE FROM \"article\" "
                    "WHERE id = $1";

    const char * const data[1] = { article_id };

    PGresult *data_result = PQexecParams(conn,command,1,NULL,data,NULL,NULL,0);

    PQclear(data_result);
    return;
}

DataResult get_article_data_by_slug(PGconn *conn, char* slug) {
    FIO_LOG_DEBUG("get_article_data_by_slug: slug: %s", slug);

    char* command = "SELECT * FROM \"article\" "
                    "WHERE slug = $1";
    const char * const data[1] = { slug };

    PGresult *data_result = PQexecParams(conn,command,1,NULL,data,NULL,NULL,0);

    DataResult result = get_data_result(data_result, map_article_data);

    PQclear(data_result);
    return result;
}

int get_all_articles_count(PGconn *conn) {
    FIO_LOG_DEBUG("get_all_articles_count");

    char *command = "SELECT COUNT(*) FROM article";

    PGresult *data_result = PQexecParams(conn,command,0,NULL,NULL,NULL,NULL,0);

    int result = get_article_count_result(data_result);

    PQclear(data_result);
    return result;
}

int get_all_followed_articles_count(PGconn *conn, int user_id) {
    FIO_LOG_DEBUG("get_all_followed_articles_count: user_id=%d", user_id);

    char *command = "SELECT COUNT(*) FROM article WHERE created_by IN (SELECT user_follow_id FROM follow WHERE user_id = $1)";
    char *data[1];

    char user_id_str[20];
    sprintf(user_id_str, "%d", user_id);
    data[0] = user_id_str;

    PGresult *data_result = PQexecParams(conn,command,1,NULL,data,NULL,NULL,0);

    int result = get_article_count_result(data_result);

    PQclear(data_result);
    return result;
}

DataResult get_all_followed_articles(PGconn *conn, int user_id, int limit, int offset) {
    FIO_LOG_DEBUG("get_all_followed_articles: user_id=%d, limit=%d, offset=%d", user_id, limit, offset);

    char *BASE_QUERY = "SELECT * FROM article WHERE created_by IN (SELECT user_follow_id FROM follow WHERE user_id = $1)";
    char *BASE_LIMIT_QUERY = "LIMIT $%d";
    char *BASE_OFFSET_QUERY = "OFFSET $%d";
    int data_count = 1;
    char *data[3];

    char user_id_str[20];
    sprintf(user_id_str, "%d", user_id);
    char limit_str[20];
    sprintf(limit_str, "%d", limit);
    char offset_str[20];
    sprintf(offset_str, "%d", offset);

    data[0] = user_id_str;

    int command_length = strlen(BASE_QUERY) + strlen(BASE_LIMIT_QUERY) + strlen(BASE_OFFSET_QUERY) + 500;
    char command[command_length];
    memset(command, '\0', command_length);
    sprintf(command, BASE_QUERY);

    if (limit != NULL) {
        char limit_buffer[strlen(BASE_LIMIT_QUERY) + strlen(limit_str) + 500];
        memset(limit_buffer, '\0', strlen(BASE_LIMIT_QUERY) + strlen(limit_str) + 1);
        sprintf(limit_buffer, BASE_LIMIT_QUERY, data_count + 1);
        sprintf(command + strlen(command), " %s", limit_buffer);

        data[data_count] = limit_str;
        data_count += 1;
    }

    if (offset != NULL) {
        char offset_buffer[strlen(BASE_OFFSET_QUERY) + strlen(offset_str) + 500];
        memset(offset_buffer, '\0', strlen(BASE_OFFSET_QUERY) + strlen(offset_str) + 1);
        sprintf(offset_buffer, BASE_OFFSET_QUERY, data_count + 1);
        sprintf(command + strlen(command), " %s", offset_buffer);

        data[data_count] = offset_str;
        data_count += 1;
    }

    PGresult *data_result = PQexecParams(conn,command,data_count,NULL,data,NULL,NULL,0);

    DataResult result = get_data_result(data_result, map_many_article_data);

    ArticleDataRecordset *typ = result.data;
    PQclear(data_result);

    return result;
}

DataResult get_all_articles(PGconn *conn, char *author, char *tag, int limit, int offset, char* favorited) {
    FIO_LOG_DEBUG("get_all_articles: author=%s, tag=%s, limit=%d, offset=%d, favorited=%s", author, tag, limit, offset, favorited);
    
    char *BASE_AUTHOR_QUERY = "created_by = (SELECT id FROM \"user\" WHERE username = $%d)";
    char *BASE_TAG_QUERY = "id IN (SELECT article_id FROM \"tag\" JOIN article_tag ON article_tag.tag_id = tag.id WHERE name = $%d)";
    char *BASE_FAVORITED_QUERY = "id IN (SELECT article_id FROM article_favourite JOIN \"user\" ON \"user\".id = article_favourite.user_id WHERE username = $%d)";
    char *BASE_QUERY = "SELECT * FROM \"article\"";
    char *BASE_LIMIT_QUERY = "LIMIT $%d";
    char *BASE_OFFSET_QUERY = "OFFSET $%d";

    char limit_str[20];
    sprintf(limit_str, "%d", limit);

    char offset_str[20];
    sprintf(offset_str, "%d", offset);

    int max_command_length = strlen(BASE_AUTHOR_QUERY) + strlen(BASE_TAG_QUERY) + strlen(BASE_QUERY) + strlen(BASE_LIMIT_QUERY) + strlen(BASE_OFFSET_QUERY) + strlen(BASE_FAVORITED_QUERY) + 500;
    char *data[5];
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

    if (limit != NULL) {
        char limit_buffer[strlen(BASE_LIMIT_QUERY) + strlen(limit_str) + 500];
        memset(limit_buffer, '\0', strlen(BASE_LIMIT_QUERY) + strlen(limit_str) + 1);
        sprintf(limit_buffer, BASE_LIMIT_QUERY, data_count + 1);
        sprintf(command + strlen(command), " %s", limit_buffer);

        data[data_count] = limit_str;
        data_count += 1;
    }

    if (offset != NULL) {
        char offset_buffer[strlen(BASE_OFFSET_QUERY) + strlen(offset_str) + 500];
        memset(offset_buffer, '\0', strlen(BASE_OFFSET_QUERY) + strlen(offset_str) + 1);
        sprintf(offset_buffer, BASE_OFFSET_QUERY, data_count + 1);
        sprintf(command + strlen(command), " %s", offset_buffer);

        data[data_count] = offset_str;
        data_count += 1;
    }

    if (favorited != NULL) {
        if (data_count == 0) {
            sprintf(command + strlen(command), "%s", " WHERE");
        }

        if (data_count > 0) {
            sprintf(command + strlen(command), " %s ", "AND");
        }

        char favorited_buffer[strlen(BASE_FAVORITED_QUERY) + strlen(favorited) + 500];
        memset(favorited_buffer, '\0', strlen(BASE_FAVORITED_QUERY) + strlen(favorited) + 1);

        sprintf(favorited_buffer, BASE_FAVORITED_QUERY, data_count + 1);
        sprintf(command + strlen(command), " %s", favorited_buffer);

        data[data_count] = favorited;
        data_count += 1;
    }

    PGresult *data_result = PQexecParams(conn,command,data_count,NULL,data,NULL,NULL,0);

    DataResult result = get_data_result(data_result, map_many_article_data);

    PQclear(data_result);
    return result;
}

int get_article_count_by_title(PGconn *conn,char* title) {
    FIO_LOG_DEBUG("get_article_by_slug: title: %s", title);

    char* command = "SELECT COUNT(*) FROM \"article\""
                    "WHERE title = $1";
    const char * const data[1] = { title, };

    PGresult *data_result = PQexecParams(conn,command,1,NULL,data,NULL,NULL,0);

    int result = get_article_count_result(data_result);
    PQclear(data_result);

    return result;
}

int get_article_count_result(const PGresult *res) {
    ExecStatusType command_status = PQresultStatus(res);

    if (command_status == PGRES_TUPLES_OK && PQntuples(res) > 0) {
        return strtol(PQgetvalue(res, 0, 0), NULL, 10);
    } else if (command_status == PGRES_TUPLES_OK && PQntuples(res) == 0) {
        return 0;
    } else if (command_status == PGRES_FATAL_ERROR) {
        FIO_LOG_DEBUG("Database request failed: %s\n", PQresultErrorMessage(res));
    }

    return 0;
}

DataResult update_article_by_slug(PGconn *conn, char *slug, UpdateValue *update_values, size_t update_count) {
    FIO_LOG_DEBUG("update_article_by_slug");

    char *data[update_count + 1];

    char* update_updated_at = "updated_at = (now() at time zone 'utc')";

    int update_substring_length = strlen(update_updated_at) + 3;

    for (int i = 0; i < update_count; i++) {
        update_substring_length += strlen(update_values[i].value) + 500;
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
        recordset->data[i].slug = strdup(PQgetvalue(res, i, 1));
        recordset->data[i].title = strdup(PQgetvalue(res, i, 2));
        recordset->data[i].body = strdup(PQgetvalue(res, i, 3));
        recordset->data[i].description = strdup(PQgetvalue(res, i, 4));
        recordset->data[i].created_at = strdup(PQgetvalue(res, i, 5));
        recordset->data[i].updated_at = strdup(PQgetvalue(res, i, 6));
        recordset->data[i].created_by = strtol(PQgetvalue(res, i, 7), NULL, 10);
    }

    return recordset;
}

ArticleData *map_article_data(const PGresult *res) {
    ArticleData *data = malloc(sizeof *data);

    data->id = strtol(PQgetvalue(res, 0, 0), NULL, 10);
    data->slug = strdup(PQgetvalue(res, 0, 1));
    data->title = strdup(PQgetvalue(res, 0, 2));
    data->body = strdup(PQgetvalue(res, 0, 3));
    data->description = strdup(PQgetvalue(res, 0, 4));
    data->created_at = strdup(PQgetvalue(res, 0, 5));
    data->updated_at = strdup(PQgetvalue(res, 0, 6));
    data->created_by = strtol(PQgetvalue(res, 0, 7), NULL, 10);

    return data;
}
