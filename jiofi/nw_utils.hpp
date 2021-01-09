/*
 * Utility functions for network and data handling
 */

#include <iostream>
#include <curl/curl.h>

/*
 *  cb_append_data - callback for appending data from network stream.
 */
size_t cb_append_data(void *ptr, 
        size_t size, 
        size_t nmemb, 
        void *vstring) {
    std::string *pstring = (std::string*) vstring;
    pstring->append((char*) ptr, size * nmemb);
    return size * nmemb;
}


/*
 * fetch_data: Fetch data from a given URL. Performs basic curl request.
 */
std::string fetch_data(const std::string &url) {
    // Source: https://stackoverflow.com/questions/19910562/read-xml-lines-from-url

    std::string body;

    CURL *curl_handle;
    CURLcode res;

    // Initialize CURL environment
    curl_global_init(CURL_GLOBAL_ALL);

    // prepare for request
    curl_handle = curl_easy_init();
    curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, cb_append_data);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &body);

    // perform request
    res = curl_easy_perform(curl_handle);

    // cleanup
    curl_easy_cleanup(curl_handle);

    return body;
}