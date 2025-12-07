#include <iostream>
#include <chrono>
#include <thread>
#include <csignal>
#include <atomic>
#include <map>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "system_info.h"
#include "http_client.h"

std::atomic<bool> running{true};

void signal_handler(int signal) {
    std::cout << "Received signal " << signal << ", shutting down..." << std::endl;
    running = false;
}

std::string get_current_timestamp() {
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm tm_buf;
    gmtime_r(&now_time_t, &tm_buf);
    char time_str[100];
    std::strftime(time_str, sizeof(time_str), "%Y-%m-%dT%H:%M:%SZ", &tm_buf);
    return std::string(time_str);
}

int main(int argc, char* argv[]) {
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);
    
    // Параметры по умолчанию
    std::string server_url = "http://localhost:8000/metrics";
    int interval = 10; // секунд
    
    if (argc > 1) server_url = argv[1];
    if (argc > 2) interval = std::stoi(argv[2]);
    
    std::cout << "Monitoring Agent started" << std::endl;
    std::cout << "Server: " << server_url << std::endl;
    std::cout << "Interval: " << interval << "s" << std::endl;
    std::cout << "Press Ctrl+C to stop" << std::endl;
    
    HttpClient http_client;
    
    while (running) {
        try {
            // Собираем метрики
            auto cpu = SystemInfo::get_cpu_usage();
            auto memory = SystemInfo::get_memory_usage();
            auto hostname = SystemInfo::get_hostname();
            std::string timestamp = get_current_timestamp();
            
            // Формируем данные для отправки
            std::map<std::string, std::string> metrics = {
                {"hostname", hostname},
                {"timestamp", timestamp},
                {"cpu_usage", std::to_string(cpu.usage_percent)},
                {"memory_used", std::to_string(memory.used)},
                {"memory_total", std::to_string(memory.total)}
            };
            
            // Отправляем метрики
            if (http_client.post_json(server_url, metrics)) {
                std::cout << "Metrics sent: CPU=" << cpu.usage_percent 
                          << "%, Memory=" << memory.usage_percent << "%" << std::endl;
            } else {
                std::cerr << "Failed to send metrics" << std::endl;
            }
            
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
        
        // Ждем указанный интервал
        for (int i = 0; i < interval && running; ++i) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    
    std::cout << "Agent stopped" << std::endl;
    return 0;
}
