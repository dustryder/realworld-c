typedef enum {
    DATA_SUCCESS,
    DATA_DUPLICATE,
    DATA_CONSTRAINT,
    DATA_NOT_FOUND,
    DATA_UNKNOWN,
} DataStatus;

typedef struct {
    int id;
    char* email;
    char* username;
    char* password;
} UserData;

typedef struct {
    DataStatus status;
    UserData data;
} UserDataResult;

UserDataResult insertUser(char* email, char* username, char* password);
UserDataResult get_user_by_email(char* email);
UserDataResult get_user_data_by_id(int id);