typedef enum {
    CREATE_USER_SUCCESS,
    CREATE_USER_DUPLICATE,
    CREATE_USER_UNKNOWN
} CreateUserStatus;

typedef struct {
    CreateUserStatus status;
    char* result;
} RegisterUserStatus;

RegisterUserStatus register_user(char* username, char* email, char* password);