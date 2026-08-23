#include <jwt.h>
#include <time.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

char *sign_jwt(int sub) {
    const unsigned char *jwt_secret = (const unsigned char *)getenv("JWT_SECRET");

    jwt_t *jwt = NULL;

    jwt_new(&jwt);

    jwt_add_grant_int(jwt, "sub", sub);
    jwt_add_grant_int(jwt, "iat", time(NULL));
    jwt_add_grant_int(jwt, "exp", (long)time(NULL) + 3600);

    jwt_set_alg(jwt, JWT_ALG_HS256, jwt_secret, strlen((const char *)jwt_secret));
    
    char* result = jwt_encode_str(jwt);

    jwt_free(jwt);

    return result;
}

int decode_jwt_sub(char* token) {
    const unsigned char *jwt_secret = (const unsigned char *)getenv("JWT_SECRET");

    jwt_t *jwt = NULL;
    jwt_valid_t *jwt_valid = NULL;

    jwt_decode(&jwt, token, jwt_secret, strlen((const char *)jwt_secret));

    jwt_valid_new(&jwt_valid, JWT_ALG_HS256);

    jwt_validate(jwt, jwt_valid);

    int sub = jwt_get_grant_int(jwt, "sub");

    jwt_free(jwt);
    jwt_valid_free(jwt_valid);

    return sub;
}