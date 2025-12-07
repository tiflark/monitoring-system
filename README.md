# Мониторинговая система на C++

## Описание
Система мониторинга с C++ агентами для сбора метрик.

## Быстрый старт
1. Запустите инфраструктуру: `cd docker && docker-compose up -d`
2. Соберите агент: `cd agent && mkdir build && cd build && cmake .. && make`
3. Запустите агент: `./agent http://localhost:8000/metrics 10`

## Компоненты
- InfluxDB: http://localhost:8086
- Grafana: http://localhost:3000
