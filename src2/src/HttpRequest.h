// ============ HttpRequest.h ============
#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <string>
#include <map>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class HttpRequest {
private:
    std::string method;
    std::string url;
    std::map<std::string, std::string> headers;
    std::string body;

public:
    HttpRequest(const std::string& method, const std::string& url);
    
    void addHeader(const std::string& key, const std::string& value);
    void setBody(const std::string& body);
    void setBody(const json& jsonBody);
    
    std::string getMethod() const { return method; }
    std::string getUrl() const { return url; }
    std::map<std::string, std::string> getHeaders() const { return headers; }
    std::string getBody() const { return body; }
    
    bool hasBody() const;
    void print() const;
};

#endif