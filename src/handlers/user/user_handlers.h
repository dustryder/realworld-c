#include "main.h"
#include "../../lib/http_helpers.h"

#ifndef USER_HANDLERS_H
#define USER_HANDLERS_H
typedef struct {
    int status;
    char* body;
} HttpResult;

typedef struct {
    char* username;
    char* email;
    char* password;
} PostUserPayload;

typedef struct {
    OptionalValue username;
    OptionalValue email;
    OptionalValue password;
    OptionalValue bio;
    OptionalValue image;
} PutUserPayload;

typedef struct {
    char* email;
    char* password;
} PostLoginRequestPayload;

enum PostUserResult {
    POST_USER_SUCCESS,
    POST_USER_DUPLICATE,
};

void handle_post_user(http_s* h);
void handle_post_login(http_s* h);
void handle_get_user(http_s* h);
void handle_put_user(http_s* h);

char* create_post_user_success();
char* create_post_user_failure();
char* create_user_success_response(char *email, char *username, char *token, char *image, char *bio);
PostUserPayload parse_post_user_body(FIOBJ *raw_body);
PutUserPayload parse_put_user_body(FIOBJ *raw_body);
PostLoginRequestPayload parse_post_login_body(FIOBJ *body);
#endif