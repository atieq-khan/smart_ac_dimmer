
#include <web_server.h>

static const char *TAG = "WEB_SERVER";

static httpd_handle_t webServerHandle = NULL;

static const httpd_uri_t root_handler()

    void web_server_init(void)
{
    // create server with defualt settings.
    httpd_config_t webServerConfig = HTTPD_DEFAULT_CONFIG();

    // start the Webserver.
    if (httpd_start(&webServerHandle, &webServerConfig) != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to start HTTP Server");
        return false;
    }

    // Register the Root handler (aka Web page)

    const httpd_uri_t root = {
        .uri = "/",
        .method = HTTP_GET,
        .handler =,
        .user_ctx = NULL,

    };

    if (httpd_register_uri_handler(webServerHandle, ) != ESP_OK)
    {
    }
}