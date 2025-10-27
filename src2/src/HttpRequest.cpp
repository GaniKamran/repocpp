// ============ HttpRequest.cpp ============
#include "HttpRequest.h"
#include <iostream>

HttpRequest::HttpRequest(const std::string& method, const std::string& url) 
    : method(method), url(url) {}

void HttpRequest::addHeader(const std::string& key, const std::string& value) {
    headers[key] = value;
}

void HttpRequest::setBody(const std::string& body) {
    this->body = body;
}

void HttpRequest::setBody(const json& jsonBody) {
    this->body = jsonBody.dump();
}

bool HttpRequest::hasBody() const {
    return !body.empty() && (method == "POST" || method == "PUT" || method == "PATCH");
}

void HttpRequest::print() const {
    std::cout << "  Method: " << method << "\n";
    std::cout << "  URL: " << url << "\n";
    if (!headers.empty()) {
        std::cout << "  Headers:\n";
        for (const auto& [key, value] : headers) {
            std::cout << "    " << key << ": " << value << "\n";
        }
    }
    if (hasBody()) {
        std::cout << "  Body: " << body.substr(0, 100);
        if (body.length() > 100) std::cout << "...";
        std::cout << "\n";
    }
}
