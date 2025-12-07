#include "http_client.h"
#include <curl/curl.h>
#include <iostream>
#include <sstream>
#include <stdexcept>

size_t HttpClient::write_callback(void* contents, 
                                 size_t size, 
                                 size_t nmemb, 
                                 std::string* output) {
    size_t total_size = size * nmemb;
    output->append((char*)contents, total_size);
    return total_size;
}

bool HttpClient::post_json(const std::string& url, 
                          const std::map<std::string, std::string>& data) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize CURL" << std::endl;
        return false;
    }
    
    // Формируем JSON вручную (упрощенно)
    std::stringstream json;
    json << "{";
    bool first = true;
    for (const auto& [key, value] : data) {
        if (!first) json << ",";
        json << "\"" << key << "\":\"" << value << "\"";
        first = false;
    }
    json << "}";
    
    std::string json_str = json.str();
    std::string response;
    
    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_str.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, json_str.size());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    
    CURLcode res = curl_easy_perform(curl);
    
    bool success = (res == CURLE_OK);
    
    if (!success) {
        std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;
    }
    
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    
    return success;
}
