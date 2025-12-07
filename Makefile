.PHONY: help build run-infra stop-infra run-agent run-server clean

help:
	@echo "Available commands:"
	@echo "  make build        - Build C++ agent"
	@echo "  make run-infra    - Start infrastructure (InfluxDB + Grafana)"
	@echo "  make stop-infra   - Stop infrastructure"
	@echo "  make run-server   - Start Python receiver server"
	@echo "  make run-agent    - Run C++ agent"
	@echo "  make clean        - Clean build files"

build:
	@echo "Building C++ agent..."
	cd agent && mkdir -p build && cd build && cmake .. && make

run-infra:
	@echo "Starting infrastructure..."
	cd docker && docker compose up -d
	@echo "InfluxDB: http://localhost:8086 (admin:admin123)"
	@echo "Grafana: http://localhost:3000 (admin:admin)"

stop-infra:
	@echo "Stopping infrastructure..."
	cd docker && docker compose down

run-server:
	@echo "Starting receiver server..."
	cd receiver && source venv/bin/activate && uvicorn src.server:app --host 0.0.0.0 --port 8000

run-agent:
	@echo "Running agent..."
	cd agent/build && ./agent http://localhost:8000/metrics 10

clean:
	@echo "Cleaning..."
	rm -rf agent/build
