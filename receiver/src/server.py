from fastapi import FastAPI, HTTPException
from pydantic import BaseModel
from datetime import datetime
from influxdb_client import InfluxDBClient, Point
from influxdb_client.client.write_api import SYNCHRONOUS
import os
import subprocess

app = FastAPI(title="Metrics Receiver")

# Модель для метрик
class Metric(BaseModel):
    hostname: str
    timestamp: datetime
    cpu_usage: float
    memory_used: int
    memory_total: int

# Функция для получения IP InfluxDB
def get_influxdb_ip():
    try:
        # Получаем IP контейнера через docker inspect
        result = subprocess.run(
            ["docker", "inspect", "-f", "{{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}", "influxdb"],
            capture_output=True,
            text=True
        )
        if result.returncode == 0 and result.stdout.strip():
            ip = result.stdout.strip()
            print(f"Found InfluxDB IP: {ip}")
            return f"http://{ip}:8086"
    except Exception as e:
        print(f"Error getting InfluxDB IP: {e}")
    
    # Если не получилось, используем стандартный адрес
    return "http://172.18.0.2:8086"

# Инициализация InfluxDB клиента
def init_influxdb_client():
    url = get_influxdb_ip()
    print(f"Connecting to InfluxDB at: {url}")
    
    return InfluxDBClient(
        url=url,
        token="my-super-secret-token",
        org="myorg",
        timeout=30_000
    )

# Инициализируем клиент
try:
    client = init_influxdb_client()
    write_api = client.write_api(write_options=SYNCHRONOUS)
    influxdb_available = True
    print("InfluxDB client initialized successfully")
except Exception as e:
    print(f"Failed to initialize InfluxDB client: {e}")
    influxdb_available = False
    client = None
    write_api = None

@app.get("/")
def root():
    return {"message": "Metrics Receiver API", "status": "running"}

@app.get("/health")
def health():
    if influxdb_available and client and client.ping():
        return {"status": "healthy", "influxdb": "connected"}
    return {"status": "degraded", "influxdb": "disconnected"}

@app.post("/metrics")
def receive_metrics(metric: Metric):
    try:
        if not influxdb_available or not write_api:
            # Если InfluxDB недоступен, все равно принимаем метрики
            print(f"Received metrics from {metric.hostname} (InfluxDB offline)")
            return {
                "status": "accepted",
                "message": f"Metrics from {metric.hostname} received (InfluxDB offline)"
            }
        
        # Создаем точку для записи в InfluxDB
        point = Point("system_metrics") \
            .tag("hostname", metric.hostname) \
            .field("cpu_usage", metric.cpu_usage) \
            .field("memory_used", metric.memory_used) \
            .field("memory_total", metric.memory_total) \
            .time(metric.timestamp)
        
        # Записываем в InfluxDB
        write_api.write(bucket="monitoring", record=point)
        
        print(f"Successfully stored metrics from {metric.hostname}")
        
        return {
            "status": "success",
            "message": f"Metrics from {metric.hostname} stored"
        }
        
    except Exception as e:
        print(f"Error processing metrics: {e}")
        # Все равно возвращаем успех, чтобы агент продолжал работать
        return {
            "status": "accepted",
            "message": f"Metrics from {metric.hostname} received (with error)",
            "error": str(e)
        }

@app.get("/metrics/{hostname}")
def get_metrics(hostname: str, limit: int = 10):
    if not influxdb_available or not client:
        raise HTTPException(status_code=503, detail="InfluxDB not available")
    
    query = f'''
    from(bucket: "monitoring")
      |> range(start: -1h)
      |> filter(fn: (r) => r["_measurement"] == "system_metrics")
      |> filter(fn: (r) => r["hostname"] == "{hostname}")
      |> limit(n: {limit})
    '''
    
    try:
        result = client.query_api().query(query)
        
        metrics = []
        for table in result:
            for row in table.records:
                metrics.append({
                    "time": row.get_time().isoformat(),
                    "field": row.get_field(),
                    "value": row.get_value()
                })
        
        return {"hostname": hostname, "metrics": metrics}
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8000)
