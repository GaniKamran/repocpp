// ============ main.cpp ============
#include "RequestManager.h"
#include <iostream>

int main() {
    try {
        // data qovluğundakı bütün JSON faylları emal olunacaq
        RequestManager manager("test_data");
        manager.processAllFiles();
        
    } catch (const std::exception& e) {
        std::cerr << "Fatal Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}