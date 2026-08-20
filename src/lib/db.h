#include <libpq-fe.h>
#include "http_helpers.h"

#ifndef LIB_DB_H
#define LIB_DB_H

#define UNIQUE_VIOLATION 23505

typedef enum {
    DATA_SUCCESS,
    DATA_DUPLICATE,
    DATA_CONSTRAINT,
    DATA_NOT_FOUND,
    DATA_UNKNOWN,
} DataStatus;

typedef struct {
    DataStatus status;
    void *data;
    ErrorValue error;
} DataResult;

typedef void *(*Mapper)(const PGresult *res);

char *datetimestamp_to_date(char* datetimestamp);
PGconn *get_connection();
DataResult get_data_result(const PGresult *res, Mapper mapper);
#endif