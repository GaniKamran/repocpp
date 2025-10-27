// ============ JsonFileReader.cpp ============
#include "JsonFileReader.h"
#include <fstream>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

std::string JsonFileReader::readFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filepath);
    }
    
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    return content;
}

std::vector<HttpRequest> JsonFileReader::parseRequestsFromFile(const std::string& filepath) {
    std::vector<HttpRequest> requests;
    
    try {
        std::string jsonContent = readFile(filepath);
        json jsonData = json::parse(jsonContent);
        
        for (const auto& item : jsonData) {
            std::string method = item.value("method", "GET");
            std::string url = item.value("url", "");
            
            if (url.empty()) {
                std::cerr << "Warning: Skipping request with empty URL\n";
                continue;
            }
            
            HttpRequest request(method, url);
            
            // Headers əlavə et
            if (item.contains("headers") && item["headers"].is_object()) {
                for (auto& [key, value] : item["headers"].items()) {
                    request.addHeader(key, value.get<std::string>());
                }
            }
            
            // Body əlavə et (yalnız POST, PUT, PATCH üçün)
            if ((method == "POST" || method == "PUT" || method == "PATCH") 
                && item.contains("body")) {
                if (item["body"].is_object() || item["body"].is_array()) {
                    request.setBody(item["body"]);
                } else {
                    request.setBody(item["body"].get<std::string>());
                }
            }
            
            requests.push_back(request);
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error parsing file " << filepath << ": " << e.what() << "\n";
    }
    
    return requests;
}

std::vector<std::string> JsonFileReader::getAllJsonFiles(const std::string& directory) {
    std::vector<std::string> files;
    
    try {
        for (const auto& entry : fs::directory_iterator(directory)) {
            if (entry.is_regular_file() && entry.path().extension() == ".json") {
                files.push_back(entry.path().string());
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error reading directory: " << e.what() << "\n";
    }
    
    return files;
}
