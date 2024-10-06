#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "firebase.h"

#include "esp_system.h"

#define SSID "Redmi"
#define PASS "minh2603"

#define FIREBASE_HOST "fir-4bc07-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define FIREBASE_AUTH "LXYYyWaW15SmL0C318RScMf9pq3PCvHEsPWYLNSe"

firebase_t firebase;

static const char *TAG = "wifi_station";

void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        ESP_LOGI(TAG, "Disconnected. Connecting to the AP again...");
        esp_wifi_connect();
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "Got IP: %s", ip4addr_ntoa(&event->ip_info.ip));
    }
}

void wifi_init_sta(void) {
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, &instance_any_id);
    esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL, &instance_got_ip);
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = SSID,
            .password = PASS,
        },
    };
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
    esp_wifi_start();
}
void app_main(void) {
    nvs_flash_init();
    wifi_init_sta();

    firebase.host = FIREBASE_HOST;
    firebase.auth = FIREBASE_AUTH;

    while (true) {
        uint32_t random_number = esp_random();
            if (firebase_set_int(&firebase, "/set", random_number) == 0) {
            ESP_LOGI("Firebase", "Data sent successfully");
        } else {
            ESP_LOGE("Firebase", "Failed to send data");
            
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}
}