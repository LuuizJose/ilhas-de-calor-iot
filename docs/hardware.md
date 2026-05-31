# Descrição do Hardware

## Plataforma de desenvolvimento

### ESP32 DevKit V1
- Microcontrolador: Espressif ESP32
- CPU: Dual-core 240 MHz
- Wi-Fi: 802.11 b/g/n integrado
- Tensão de operação: 3.3V
- Pinos digitais: 34
- Motivo da escolha: Wi-Fi nativo, baixo custo, suporte ao protocolo MQTT

## Sensores

### DHT22 (temperatura e umidade)
- Faixa de temperatura: -40°C a +80°C
- Precisão: ±0.5°C
- Faixa de umidade: 0–100% RH
- Protocolo: Single-wire digital
- Tensão: 3.3V–5V

## Atuadores

### Módulo Relé 5V (simulado por LED no Wokwi)
- Tensão de controle: 5V
- Corrente máxima: 10A / 250VAC
- Função: acionar a microbomba de água ao detectar temperatura crítica

### Microbomba de Água 5V
- Tensão: 5V DC
- Vazão: ~120 L/h
- Aplicação: nebulização urbana / irrigação de fachadas verdes

## Conexões

| ESP32 | Componente | Descrição |
|-------|------------|-----------|
| 3V3 | DHT22 VCC | Alimentação do sensor |
| GND | DHT22 GND | Terra do sensor |
| D4 | DHT22 SDA | Dados do sensor |
| D5 | Relé IN | Controle do relé |
| GND | Relé GND | Terra do relé |
| VIN | Relé VCC | Alimentação do relé |

## Comunicação

- **Protocolo:** MQTT sobre TCP/IP
- **Transporte:** Wi-Fi 802.11
- **Broker:** Eclipse Mosquitto / HiveMQ
- **Porta:** 1883