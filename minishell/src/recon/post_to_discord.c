#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <curl/curl.h>
#include "minishell.h"

void send_discord_file(const char *webhook_url, const char *file_path, const char *message) {
    CURL *curl;
    CURLcode res;
    struct curl_httppost *form = NULL;
    struct curl_httppost *lastptr = NULL;

    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if (curl) {
        // Set the URL for the POST request
        curl_easy_setopt(curl, CURLOPT_URL, webhook_url);

        // Add the content field
        curl_formadd(&form,
                     &lastptr,
                     CURLFORM_COPYNAME, "content",
                     CURLFORM_COPYCONTENTS, message,
                     CURLFORM_END);

        // Add the file field
        curl_formadd(&form,
                     &lastptr,
                     CURLFORM_COPYNAME, "file",
                     CURLFORM_FILE, file_path,
                     CURLFORM_END);

        // Set the form info
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, form);

        // Perform the request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        // Cleanup
        curl_easy_cleanup(curl);
        curl_formfree(form);
    }

    // Cleanup global libcurl
    curl_global_cleanup();
}
