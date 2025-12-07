cmake_minimum_required(VERSION 3.15)
project(MonitoringAgent VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Находим необходимые библиотеки
find_package(CURL REQUIRED)

# Исходные файлы
set(SOURCES
    src/main.cpp
    src/http_client.cpp
    src/system_info.cpp
)

add_executable(agent ${SOURCES})

# Подключаем библиотеки
target_include_directories(agent PRIVATE src)
target_link_libraries(agent PRIVATE 
    ${CURL_LIBRARIES}
)

# Установка
install(TARGETS agent DESTINATION bin)
