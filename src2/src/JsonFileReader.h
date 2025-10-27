// ============ JsonFileReader.h ============
#ifndef JSON_FILE_READER_H
#define JSON_FILE_READER_H

#include <string>
#include <vector>
#include "HttpRequest.h"
#include <nlohmann/json.hpp>

class JsonFileReader {
private:
    std::string readFile(const std::string& filepath);

public:
    std::vector<HttpRequest> parseRequestsFromFile(const std::string& filepath);
    std::vector<std::string> getAllJsonFiles(const std::string& directory);
};

#endif
