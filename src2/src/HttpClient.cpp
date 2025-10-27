// ============ HttpClient.cpp ============
#include "HttpClient.h"
#include <chrono>

size_t HttpClient::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

HttpClient::HttpClient() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

HttpClient::~HttpClient() {
    curl_global_cleanup();
}

HttpResponse HttpClient::execute(const HttpRequest& request) {
    CURL* curl = curl_easy_init();
    std::string responseBody;
    int statusCode = -1;
    
    auto startTime = std::chrono::high_resolution_clock::now();
    
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, request.getUrl().c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBody);
        
        // SSL sertifikat yoxlamasını deaktiv et (development üçün)
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        
        // Method ayarları
        if (request.getMethod() == "POST") {
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
            if (request.hasBody()) {
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request.getBody().c_str());
            }
        } else if (request.getMethod() == "PUT") {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
            if (request.hasBody()) {
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request.getBody().c_str());
            }
        } else if (request.getMethod() == "DELETE") {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
        } else if (request.getMethod() == "PATCH") {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
            if (request.hasBody()) {
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request.getBody().c_str());
            }
        }
        // GET default-dur, əlavə ayar lazım deyil
        
        // Headers əlavə et
        struct curl_slist* headerList = nullptr;
        for (const auto& [key, value] : request.getHeaders()) {
            std::string header = key + ": " + value;
            headerList = curl_slist_append(headerList, header.c_str());
        }
        if (headerList) {
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerList);
        }
        
        // Request göndər
        CURLcode res = curl_easy_perform(curl);
        
        if (res == CURLE_OK) {
            long httpCode = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
            statusCode = static_cast<int>(httpCode);
        } else {
            responseBody = "Error: " + std::string(curl_easy_strerror(res));
        }
        
        if (headerList) curl_slist_free_all(headerList);
        curl_easy_cleanup(curl);
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    double duration = std::chrono::duration<double>(endTime - startTime).count();
    
    return HttpResponse(statusCode, responseBody, duration);
}
