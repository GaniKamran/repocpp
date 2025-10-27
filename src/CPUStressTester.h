#ifndef CPUSTRESSTESTER_H
#define CPUSTRESSTESTER_H

#include <thread>
#include <vector>
#include <atomic>

class CPUStressTester {
private:
    std::atomic<bool> running;
    std::vector<std::thread> threads;
    std::atomic<unsigned long long> operations;

    void stressCore(int coreId);

public:
    CPUStressTester();
    ~CPUStressTester();
    
    void start();
    void showStats();
    void stop();
    bool isRunning() const;
};

#endif