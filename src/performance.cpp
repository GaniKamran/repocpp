#include "CPUStressTester.h"
#include <iostream>
#include <csignal>

CPUStressTester* globalTester = nullptr;

void signalHandler(int signum) {
    if (globalTester) {
        globalTester->stop();
    }
    exit(signum);
}

int main() {
    std::cout << "=== CPU Maksimum Yükləyici ===\n\n";
    
    CPUStressTester tester;
    globalTester = &tester;
    
    signal(SIGINT, signalHandler);
    
    tester.start();
    tester.showStats();
    
    return 0;
}
