// ============ RequestManager.cpp ============
#include "RequestManager.h"
#include <iostream>
#include <iomanip>
#include <ctime>

RequestManager::RequestManager(const std::string& dataDir, const std::string& logFilePath) 
    : dataDirectory(dataDir) {
    logFile.open(logFilePath, std::ios::out | std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Warning: Could not open log file: " << logFilePath << "\n";
    } else {
        // Log başlıq məlumatı
        auto now = std::time(nullptr);
        auto tm = *std::localtime(&now);
        logFile << "\n" << std::string(80, '=') << "\n";
        logFile << "LOG SESSION STARTED: " << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "\n";
        logFile << std::string(80, '=') << "\n";
    }
}

RequestManager::~RequestManager() {
    if (logFile.is_open()) {
        auto now = std::time(nullptr);
        auto tm = *std::localtime(&now);
        logFile << "\n" << std::string(80, '=') << "\n";
        logFile << "LOG SESSION ENDED: " << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "\n";
        logFile << std::string(80, '=') << "\n\n";
        logFile.close();
    }
}

void RequestManager::log(const std::string& message) {
    if (logFile.is_open()) {
        logFile << message;
        logFile.flush(); // Dərhal yaz
    }
}

void RequestManager::logAndPrint(const std::string& message) {
    std::cout << message;
    log(message);
}

void RequestManager::processFile(const std::string& filepath) {
    std::string separator = "\n" + std::string(80, '=') + "\n";
    std::string header = "Processing File: " + filepath + "\n";
    
    logAndPrint(separator);
    logAndPrint(header);
    logAndPrint(std::string(80, '=') + "\n\n");
    
    auto requests = reader.parseRequestsFromFile(filepath);
    
    RequestResult result;
    result.filename = filepath;
    result.totalRequests = requests.size();
    result.successCount = 0;
    result.failCount = 0;
    result.totalTime = 0;
    
    for (size_t i = 0; i < requests.size(); i++) {
        std::stringstream ss;
        ss << "Request #" << (i + 1) << " of " << requests.size() << ":\n";
        logAndPrint(ss.str());
        
        // Request məlumatlarını logla
        ss.str("");
        ss << "  Method: " << requests[i].getMethod() << "\n";
        ss << "  URL: " << requests[i].getUrl() << "\n";
        
        if (!requests[i].getHeaders().empty()) {
            ss << "  Headers:\n";
            for (const auto& [key, value] : requests[i].getHeaders()) {
                ss << "    " << key << ": " << value << "\n";
            }
        }
        
        if (requests[i].hasBody()) {
            std::string body = requests[i].getBody();
            ss << "  Body: " << body.substr(0, 100);
            if (body.length() > 100) ss << "...";
            ss << "\n";
        }
        logAndPrint(ss.str());
        
        HttpResponse response = client.execute(requests[i]);
        
        // Response məlumatlarını logla
        ss.str("");
        ss << "  Status Code: " << response.getStatusCode();
        if (response.isSuccess()) {
            ss << " ✓ SUCCESS\n";
            result.successCount++;
        } else {
            ss << " ✗ FAILED\n";
            result.failCount++;
        }
        ss << "  Response Time: " << std::fixed << std::setprecision(3) 
           << response.getResponseTime() << " seconds\n";
        
        std::string respBody = response.getBody();
        ss << "  Response Body: " << respBody.substr(0, 150);
        if (respBody.length() > 150) ss << "...";
        ss << "\n\n";
        
        logAndPrint(ss.str());
        
        result.totalTime += response.getResponseTime();
    }
    
    results.push_back(result);
}

void RequestManager::processAllFiles() {
    auto files = reader.getAllJsonFiles(dataDirectory);
    
    if (files.empty()) {
        std::cout << "No JSON files found in directory: " << dataDirectory << "\n";
        return;
    }
    
    std::cout << "\nFound " << files.size() << " JSON file(s) to process\n";
    
    for (const auto& file : files) {
        processFile(file);
    }
    
    printSummary();
}

void RequestManager::printSummary() const {
    std::cout << "\n" << std::string(80, '=') << "\n";
    std::cout << "SUMMARY REPORT\n";
    std::cout << std::string(80, '=') << "\n\n";
    
    int totalRequests = 0;
    int totalSuccess = 0;
    int totalFail = 0;
    double totalTime = 0;
    
    for (const auto& result : results) {
        std::cout << "File: " << result.filename << "\n";
        std::cout << "  Total Requests: " << result.totalRequests << "\n";
        std::cout << "  Successful: " << result.successCount << " ✓\n";
        std::cout << "  Failed: " << result.failCount << " ✗\n";
        std::cout << "  Total Time: " << std::fixed << std::setprecision(3) 
                  << result.totalTime << " seconds\n\n";
        
        totalRequests += result.totalRequests;
        totalSuccess += result.successCount;
        totalFail += result.failCount;
        totalTime += result.totalTime;
    }
    
    std::cout << std::string(80, '-') << "\n";
    std::cout << "OVERALL STATISTICS:\n";
    std::cout << "  Total Files Processed: " << results.size() << "\n";
    std::cout << "  Total Requests: " << totalRequests << "\n";
    std::cout << "  Total Successful: " << totalSuccess << " ✓\n";
    std::cout << "  Total Failed: " << totalFail << " ✗\n";
    std::cout << "  Success Rate: " << std::fixed << std::setprecision(1)
              << (totalRequests > 0 ? (totalSuccess * 100.0 / totalRequests) : 0) << "%\n";
    std::cout << "  Total Time: " << std::fixed << std::setprecision(3) 
              << totalTime << " seconds\n";
    std::cout << std::string(80, '=') << "\n";
}
