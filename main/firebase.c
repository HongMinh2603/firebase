#include "firebase.h"

esp_err_t firebase_set_int(firebase_t* firebase, const char* path, int value) {
    char url[256];
    snprintf(url, sizeof(url), "https://%s%s.json?auth=%s", firebase->host, path, firebase->auth);

    char data[64];
    snprintf(data, sizeof(data), "{\"value\": %d}", value);

    esp_http_client_config_t config = {
        .url = url,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    esp_http_client_set_method(client, HTTP_METHOD_PUT);
    esp_http_client_set_post_field(client, data, strlen(data));
    esp_err_t err = esp_http_client_perform(client);

    esp_http_client_cleanup(client);
    return err;
}
