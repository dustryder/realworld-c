typedef enum {
    DataStatusSuccess,
    DataStatusDuplicate,
    DataStatusConstraint,
    DataStatusUnknown
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