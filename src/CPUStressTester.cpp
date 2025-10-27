#include "CPUStressTester.h"
#include "MathCalculator.h"
#include <iostream>
#include <chrono>

CPUStressTester::CPUStressTester() : running(false), operations(0) {}

CPUStressTester::~CPUStressTester() {
    stop();
}

void CPUStressTester::stressCore(int coreId) {
    unsigned long long localOps = 0;
    
    while (running.load()) {
        // MathCalculator-dan hesablama funksiyasını çağır
        MathCalculator::performHeavyCalculations();
        
        localOps += 11000; // Hər iterasiyada ~11000 əməliyyat
        operations.fetch_add(localOps);
        localOps = 0;
    }
}

void CPUStressTester::start() {
    if (running) {
        std::cout << "Artıq işləyir!\n";
        return;
    }

    running = true;
    operations = 0;
    
    unsigned int numCores = std::thread::hardware_concurrency();
    std::cout << "CPU Nüvələri: " << numCores << "\n";
    std::cout << "Maksimum yükləmə başladı...\n";
    std::cout << "Dayandırmaq üçün Ctrl+C basın\n\n";

    for (unsigned int i = 0; i < numCores; i++) {
        threads.emplace_back(&CPUStressTester::stressCore, this, i);
    }
}

void CPUStressTester::showStats() {
    auto lastOps = operations.load();
    auto lastTime = std::chrono::steady_clock::now();
    
    while (running.load()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        auto currentOps = operations.load();
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            currentTime - lastTime).count();
        
        auto opsPerSec = (currentOps - lastOps) * 1000.0 / elapsed;
        
        std::cout << "Əməliyyatlar: " << currentOps 
                  << " | Sürət: " << static_cast<long long>(opsPerSec) << " ops/s"
                  << " | CPU: 100%     \r" << std::flush;
        
        lastOps = currentOps;
        lastTime = currentTime;
    }
}

void CPUStressTester::stop() {
    if (!running) return;
    
    std::cout << "\n\nDayandırılır...\n";
    running = false;
    
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    threads.clear();
    std::cout << "Dayandırıldı. Ümumi əməliyyat: " << operations.load() << "\n";
}

bool CPUStressTester::isRunning() const {
    return running.load();
}
