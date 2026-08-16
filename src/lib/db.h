#include <libpq-fe.h>

#ifndef LIB_DB_H
#define LIB_DB_H

#define UNIQUE_VIOLATION 23505

PGconn *get_connection();
#endif