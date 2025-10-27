// ============ RequestManager.h ============
#ifndef REQUEST_MANAGER_H
#define REQUEST_MANAGER_H

#include <vector>
#include <string>
#include <fstream>
#include "HttpClient.h"
#include "JsonFileReader.h"

class RequestManager {
private:
    HttpClient client;
    JsonFileReader reader;
    std::string dataDirectory;
    std::ofstream logFile;
    
    struct RequestResult {
        std::string filename;
        int totalRequests;
        int successCount;
        int failCount;
        double totalTime;
    };
    
    std::vector<RequestResult> results;
    
    void log(const std::string& message);
    void logAndPrint(const std::string& message);

public:
    RequestManager(const std::string& dataDir, const std::string& logFilePath = "log_data_test_api.log");
    ~RequestManager();
    
    void processAllFiles();
    void processFile(const std::string& filepath);
    void printSummary() const;
};

#endif
