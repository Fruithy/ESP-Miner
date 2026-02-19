#include "esp_http_client.h"
#include "esp_log.h"
#include <string>

static float btc_price_nok = 0;

float get_btc_price() {
    return btc_price_nok;
}

// HTTP event handler (required but we don’t process events)
esp_err_t http_event_handler(esp_http_client_event_t *evt) {
    return ESP_OK;
}

void fetch_btc_price() {
    esp_http_client_config_t config = {
        .url = "https://api.coingecko.com/api/v3/simple/price?ids=bitcoin&vs_currencies=nok",
        .event_handler = http_event_handler,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK) {
        char buffer[256];
        int len = esp_http_client_read_response(client, buffer, sizeof(buffer)-1);
        if (len > 0) {
            buffer[len] = 0;
            char *p = strstr(buffer, "nok");
            if (p) {
                btc_price_nok = atof(p + 5); // crude parse, works for {"bitcoin":{"nok":value}}
            }
        }
    }

    esp_http_client_cleanup(client);
}
