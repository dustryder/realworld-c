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


typedef enum {
    LOGIN_USER_SUCCESS,
    LOGIN_USER_BAD_PASSWORD,
    LOGIN_USER_UNKNOWN,
} LoginUserStatus;

typedef struct {
    char* email;
    char* token;
    char* username;
    char* bio;
    char* image;
} LoginUserData;

typedef struct {
    LoginUserStatus status;
    LoginUserData result;
} LoginUserResult;

LoginUserResult login(char* email, char* password);


typedef enum {
    GET_USER_SUCCESS,
    GET_USER_UNKNOWN,
} GetUserStatus;

typedef struct {
    char* email;
    char* token;
    char* username;
    char* bio;
    char* image;
} GetUserData;

typedef struct {
    GetUserStatus status;
    GetUserData result;
} GetUserByIdResult;

GetUserByIdResult get_user_by_id(int id);