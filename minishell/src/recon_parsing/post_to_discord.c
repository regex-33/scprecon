#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

void send_discord_file(const char *webhook_url, const char *file_path, const char *message) {
    CURL *curl;
    CURLcode res;
    curl_mime *mime;
    curl_mimepart *part;

    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if (curl) {
        // Set the URL for the POST request
        curl_easy_setopt(curl, CURLOPT_URL, webhook_url);

        // Create the mime handle
        mime = curl_mime_init(curl);

        // Add the content field
        part = curl_mime_addpart(mime);
        curl_mime_name(part, "content");
        curl_mime_data(part, message, CURL_ZERO_TERMINATED);

        // Add the file field
        part = curl_mime_addpart(mime);
        curl_mime_name(part, "file");
        curl_mime_filedata(part, file_path);

        // Set the mime post data
        curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);

        // Perform the request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        // Cleanup
        curl_mime_free(mime);
        curl_easy_cleanup(curl);
    }

    // Cleanup global libcurl
    curl_global_cleanup();
}

