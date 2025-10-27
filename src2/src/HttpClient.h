// ============ HttpClient.h ============
#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include "HttpRequest.h"
#include "HttpResponse.h"
#include <curl/curl.h>

class HttpClient {
private:
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp);

public:
    HttpClient();
    ~HttpClient();
    
    HttpResponse execute(const HttpRequest& request);
};

#endif