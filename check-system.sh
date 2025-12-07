#!/bin/bash
echo "=== Checking Monitoring System ==="
echo ""

# 1. Проверяем текущую директорию
echo "1. Current directory:"
pwd
echo ""

# 2. Проверяем Docker
echo "2. Docker status:"
if command -v docker &> /dev/null; then
    docker --version
else
    echo "ERROR: Docker not found!"
fi
echo ""

# 3. Проверяем Docker Compose (плагин)
echo "3. Docker Compose (plugin):"
if command -v docker compose &> /dev/null; then
    docker compose version
else
    echo "WARNING: docker compose not found, trying docker-compose..."
    if command -v docker-compose &> /dev/null; then
        docker-compose --version
        echo "Using docker-compose (legacy)"
    else
        echo "ERROR: Neither docker compose nor docker-compose found!"
    fi
fi
echo ""

# 4. Проверяем структуру проекта
echo "4. Project structure:"
ls -la
echo ""
echo "docker folder:"
ls -la docker/
echo ""
echo "agent folder:"
ls -la agent/
echo ""
echo "receiver folder:"
ls -la receiver/
echo ""

# 5. Проверяем контейнеры
echo "5. Running containers:"
docker ps --format "table {{.Names}}\t{{.Status}}\t{{.Ports}}"
echo ""

# 6. Проверяем Python
echo "6. Python status:"
python3 --version
echo ""

# 7. Проверяем C++ компилятор
echo "7. C++ compiler:"
g++ --version | head -1
echo ""

# 8. Проверяем, что порты свободны
echo "8. Ports in use:"
for port in 3000 8086 8000; do
    if ss -tulpn | grep ":$port" > /dev/null; then
        echo "Port $port: IN USE"
    else
        echo "Port $port: FREE"
    fi
done
echo ""

echo "=== Check complete ==="
