#!/bin/bash
echo "=== Starting Monitoring System ==="

echo "1. Starting infrastructure..."
cd docker && docker compose up -d
sleep 5

echo "2. Building agent..."
cd ../agent && mkdir -p build && cd build && cmake .. && make
sleep 2

echo "3. Starting receiver server in background..."
cd ../../receiver
source venv/bin/activate
nohup uvicorn src.server:app --host 0.0.0.0 --port 8000 > server.log 2>&1 &
SERVER_PID=$!
sleep 3

echo "4. Starting agent..."
cd ../agent/build
./agent http://localhost:8000/metrics 10 &
AGENT_PID=$!

echo ""
echo "=== System is running! ==="
echo "InfluxDB: http://localhost:8086"
echo "Grafana: http://localhost:3000"
echo "Receiver API: http://localhost:8000"
echo ""
echo "Logs:"
echo "  Server: tail -f receiver/server.log"
echo "  Agent: running in foreground"
echo ""
echo "Press Ctrl+C to stop all services"

# Ожидаем Ctrl+C
trap 'kill $AGENT_PID; kill $SERVER_PID; cd docker && docker compose down; echo "System stopped"' INT TERM
wait $AGENT_PID
