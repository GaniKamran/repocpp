// ============ HttpResponse.cpp ============
#include "HttpResponse.h"
#include <iostream>
#include <iomanip>

HttpResponse::HttpResponse(int code, const std::string& body, double time)
    : statusCode(code), body(body), responseTime(time) {}

bool HttpResponse::isSuccess() const {
    return statusCode >= 200 && statusCode < 300;
}

void HttpResponse::print() const {
    std::cout << "  Status Code: " << statusCode;
    if (isSuccess()) {
        std::cout << " ✓ SUCCESS";
    } else {
        std::cout << " ✗ FAILED";
    }
    std::cout << "\n";
    std::cout << "  Response Time: " << std::fixed << std::setprecision(3) 
              << responseTime << " seconds\n";
    std::cout << "  Response Body: " << body.substr(0, 150);
    if (body.length() > 150) std::cout << "...";
    std::cout << "\n";
}
