typedef struct {
    int status;
    char* body;
} HttpResult;

typedef struct {
    char* username;
    char* email;
    char* password;
} PostUserPayload;

enum PostUserResult {
    POST_USER_SUCCESS,
    POST_USER_DUPLICATE,
};

HttpResult handle_post_user(http_s* h);
char* create_post_user_success();
char* create_post_user_failure();
PostUserPayload parse_post_user_body(FIOBJ *raw_body);