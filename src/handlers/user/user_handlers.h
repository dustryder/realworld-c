#include "main.h"
#include "../../lib/http_helpers.h"

#ifndef USER_HANDLERS_H
#define USER_HANDLERS_H
typedef struct {
    OptionalValue username;
    OptionalValue email;
    OptionalValue password;
} PostUserPayload;

typedef struct {
    OptionalValue username;
    OptionalValue email;
    OptionalValue password;
    OptionalValue bio;
    OptionalValue image;
} PutUserPayload;

typedef struct {
    OptionalValue email;
    OptionalValue password;
} PostLoginRequestPayload;

enum PostUserResult {
    POST_USER_SUCCESS,
    POST_USER_DUPLICATE,
};

void handle_post_user(http_s* h);
void handle_post_login(http_s* h);
void handle_get_user(http_s* h);
void handle_put_user(http_s* h);

char* create_user_success_response(char *email, char *username, char *token, char *image, char *bio);
#endif