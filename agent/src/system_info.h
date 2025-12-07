#ifndef SYSTEM_INFO_H
#define SYSTEM_INFO_H

#include <string>
#include <cstdint>

namespace SystemInfo {
    struct CPUStats {
        double usage_percent;
    };
    
    struct MemoryInfo {
        uint64_t total;
        uint64_t used;
        double usage_percent;
    };
    
    CPUStats get_cpu_usage();
    MemoryInfo get_memory_usage();
    std::string get_hostname();
}

#endif
