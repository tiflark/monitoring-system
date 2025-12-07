#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <string>
#include <map>

class HttpClient {
public:
    HttpClient() = default;
    ~HttpClient() = default;
    
    bool post_json(const std::string& url, 
                   const std::map<std::string, std::string>& data);
    
private:
    static size_t write_callback(void* contents, 
                                 size_t size, 
                                 size_t nmemb, 
                                 std::string* output);
};

#endif
