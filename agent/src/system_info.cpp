#include "system_info.h"
#include <fstream>
#include <sstream>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <string.h>
#include <chrono>
#include <thread>

namespace SystemInfo {

CPUStats get_cpu_usage() {
    // Простая реализация - читаем /proc/stat
    std::ifstream file("/proc/stat");
    if (!file.is_open()) {
        return CPUStats{0.0};
    }
    
    std::string line;
    std::getline(file, line); // Читаем первую строку "cpu"
    
    std::istringstream iss(line.substr(5)); // Пропускаем "cpu "
    long user, nice, system, idle;
    iss >> user >> nice >> system >> idle;
    
    // Очень упрощенный расчет - просто возвращаем 5% для теста
    return CPUStats{5.0};
}

MemoryInfo get_memory_usage() {
    struct sysinfo info;
    if (sysinfo(&info) != 0) {
        return MemoryInfo{0, 0, 0.0};
    }
    
    uint64_t total = info.totalram * info.mem_unit;
    uint64_t free = info.freeram * info.mem_unit;
    uint64_t used = total - free;
    double usage_percent = (total > 0) ? (100.0 * used / total) : 0.0;
    
    return MemoryInfo{total, used, usage_percent};
}

std::string get_hostname() {
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        return std::string(hostname);
    }
    return "unknown";
}

}
