#include "unity.h"
#include "../src/lib/string_helpers.h"
#include "../src/services/articles/articles_services.h"

static DataResult get_article_result;

// Stubbed methods
DataResult get_all_articles(PGconn *conn, char *author, char *tag, int limit, int offset, char *favorited) {}
DataResult get_all_followed_articles(PGconn *conn, int user_id, int limit, int offset) {}
int get_article_count_by_title(PGconn *conn, char *title) {}
int get_all_articles_count(PGconn *conn) {}
int get_article_favorite_count(PGconn *conn, char* slug) {}
int get_user_favorites_article(PGconn *conn, int user_id, char* slug) {}
int get_all_followed_articles_count(PGconn *conn, int user_id) {}
DataResult update_article_by_slug(PGconn *conn, char *slug, UpdateValue *update_values, size_t update_count) {}
void delete_article_by_id(PGconn *conn, int id) {}
DataResult insert_article(PGconn *conn, char* slug, char* title, char* description, char* body, int created_by) {}
void insert_article_favorite(PGconn *conn, int user_id, char* slug) {}
void delete_article_favorite(PGconn *conn, int user_id, char* slug) {}
void free_ArticleData(ArticleData *data) {}

// Mock implementation
DataResult get_article_data_by_slug(PGconn *conn, char *slug) {
    return get_article_result;
}

// Test data factory
ArticleData *create_article_data(int user_id) {
    ArticleData *fake_article_data = malloc(sizeof(*fake_article_data));
    fake_article_data->created_by = user_id;
    fake_article_data->body = strdup("");
    fake_article_data->created_at = strdup("");
    fake_article_data->description = strdup("");
    fake_article_data->slug = strdup("");
    fake_article_data->title = strdup("");
    fake_article_data->updated_at = strdup("");

    return fake_article_data;
}

void setUp(void) {
    memset(&get_article_result, 0, sizeof(get_article_result));
}

void tearDown(void) {
}

void test_sluggify_should_turn_string_into_slug(void) {

    // Arrange
    char *title = "My New Title!";
    int title_count = 0;
    char *expected_result = "my-new-title-0";

    // Act
    char *result = sluggify(title, title_count);

    // Assert
    TEST_ASSERT_EQUAL_STRING(result, expected_result);
}


void test_delete_article_service_returns_not_found_error_when_article_doesnt_exist(void) {

    // Arrange
    get_article_result.status = DATA_NOT_FOUND;
    PGconn *conn = NULL;
    char *article_slug = "potato";
    int user_id = 11;

    // Act
    ArticleServiceResult result = delete_article(conn, article_slug, user_id);

    // Assert
    TEST_ASSERT_EQUAL_INT(SERVICE_NOT_FOUND, result.status);
}

void test_delete_article_service_returns_UNAUTHORIZED_when_user_is_not_creator(void) {

    // Arrange
    get_article_result.status = DATA_SUCCESS;
    get_article_result.data = create_article_data(99);
    PGconn *conn = NULL;
    char *article_slug = "potato";
    int user_id = 11;

    // Act
    ArticleServiceResult result = delete_article(conn, article_slug, user_id);

    // Assert
    TEST_ASSERT_EQUAL_INT(SERVICE_UNAUTHORIZED, result.status);
}

void test_delete_article_service_returns_SUCCESS_when_user_is_creator(void) {

    // Arrange
    get_article_result.status = DATA_SUCCESS;
    PGconn *conn = NULL;
    char *article_slug = "potato";
    int user_id = 11;
    get_article_result.data = create_article_data(user_id);

    // Act
    ArticleServiceResult result = delete_article(conn, article_slug, user_id);

    // Assert
    TEST_ASSERT_EQUAL_INT(SERVICE_SUCCESS, result.status);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_sluggify_should_turn_string_into_slug);
    RUN_TEST(test_delete_article_service_returns_not_found_error_when_article_doesnt_exist);
    RUN_TEST(test_delete_article_service_returns_UNAUTHORIZED_when_user_is_not_creator);
    RUN_TEST(test_delete_article_service_returns_SUCCESS_when_user_is_creator);
    return UNITY_END();
}