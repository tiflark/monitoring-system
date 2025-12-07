#!/bin/bash
echo "=== Checking Monitoring System ==="
echo ""

# 1. Проверяем Docker
echo "1. Docker status:"
docker --version
echo "Docker Compose (plugin):"
docker compose version
echo ""

# 2. Проверяем контейнеры
echo "2. Running containers:"
docker ps
echo ""

# 3. Проверяем папки
echo "3. Project structure:"
ls -la ~/monitoring-system/
echo ""

# 4. Проверяем Python сервер
echo "4. Python server check:"
cd ~/monitoring-system/receiver && python3 -c "import fastapi; print('FastAPI:', fastapi.__version__)"
echo ""

# 5. Проверяем C++ компилятор
echo "5. C++ compiler:"
g++ --version | head -1
echo ""

# 6. Быстрый тест агента
echo "6. Quick build test:"
cd ~/monitoring-system/agent
mkdir -p build_test && cd build_test
cmake .. 2>&1 | tail -5
echo ""

echo "=== Check complete ==="
