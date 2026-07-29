#include "users_service.h"
#include "../data/users.h"

char* createUser(char* email, char* username, char* password) {

    insertUser(email, username, password);
}