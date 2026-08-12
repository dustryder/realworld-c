#include <jwt.h>
#include <time.h>
#include <stddef.h>
#include <string.h>

char *sign_jwt(int sub) {
    const unsigned char *secret = "LSatqj0GuxXGgtRX7LiKu9GM4uyVfUqjS9keNuL574Y";
    jwt_t *jwt = NULL;
    int res;

    jwt_new(&jwt);

    jwt_add_grant_int(jwt, "sub", sub);
    jwt_add_grant_int(jwt, "iat", time(NULL));
    jwt_add_grant_int(jwt, "exp", (long)time(NULL) + 3600);

    jwt_set_alg(jwt, JWT_ALG_HS256, secret, strlen(secret));
    
    char* result = jwt_encode_str(jwt);

    jwt_free(jwt);

    return result;
}

int decode_jwt_sub(char* token) {
    const unsigned char *secret = "LSatqj0GuxXGgtRX7LiKu9GM4uyVfUqjS9keNuL574Y";
    jwt_t *jwt = NULL;
    jwt_valid_t *jwt_valid = NULL;

    int result = jwt_decode(&jwt, token, secret, strlen(secret));

    result = jwt_valid_new(&jwt_valid, JWT_ALG_HS256);

    result = jwt_validate(jwt, jwt_valid);

    int sub = jwt_get_grant_int(jwt, "sub");

    return sub;
}