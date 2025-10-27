// ============ HttpResponse.h ============
#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <string>

class HttpResponse {
private:
    int statusCode;
    std::string body;
    double responseTime;

public:
    HttpResponse(int code, const std::string& body, double time);
    
    int getStatusCode() const { return statusCode; }
    std::string getBody() const { return body; }
    double getResponseTime() const { return responseTime; }
    
    bool isSuccess() const;
    void print() const;
};

#endif
