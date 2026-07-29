typedef struct {
    char* username;
    char* email;
    char* password;
} PostUserPayload;

void handle_post_user(http_s* h);