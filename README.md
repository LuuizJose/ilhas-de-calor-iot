# Ilhas de Calor e Saúde Urbana — IoT com ESP32

Protótipo de baixo custo para monitoramento de variáveis climáticas em áreas urbanas densas, desenvolvido na Universidade Presbiteriana Mackenzie.

## Descrição

O sistema utiliza um ESP32 integrado ao sensor DHT22 para detectar temperatura e umidade. Ao identificar temperaturas críticas (acima de 30°C), aciona automaticamente um sistema de resfriamento (bomba/relé) e transmite os dados via MQTT para monitoramento remoto em tempo real.

## Como reproduzir

### Simulação (Wokwi)
1. Acesse [wokwi.com](https://wokwi.com) e crie uma conta
2. Clique em **New Project** → **ESP32**
3. Cole o conteúdo de `codigo/sketch.ino` no editor
4. Cole o conteúdo de `simulacao/diagram.json` no diagram.json
5. Adicione as bibliotecas em `libraries.txt`
6. Clique em **Play**
7. Clique no DHT22 e arraste a temperatura acima de 30°C para ver a bomba acionar

### Hardware real
1. Monte o circuito conforme descrito em `docs/hardware.md`
2. Abra o `sketch.ino` na Arduino IDE
3. Instale as bibliotecas listadas em `libraries.txt`
4. Selecione a placa **ESP32 Dev Module**
5. Faça o upload para a placa

## Estrutura do repositório

ilhas-de-calor-iot/
├── codigo/
│   ├── sketch.ino       # Código principal
│   └── libraries.txt    # Bibliotecas necessárias
├── simulacao/
│   └── diagram.json     # Circuito para simulação no Wokwi
└── docs/
└── hardware.md      # Descrição do hardware


## Protocolo MQTT

- **Broker:** broker.hivemq.com (público, porta 1883)
- **Tópicos:**

| Tópico | Descrição |
|--------|-----------|
| `mackenzie/ilhas-calor/temperatura` | Publica temperatura em °C |
| `mackenzie/ilhas-calor/umidade` | Publica umidade em % |
| `mackenzie/ilhas-calor/bomba` | Status da bomba (LIGADA/DESLIGADA) |
| `mackenzie/ilhas-calor/comando` | Recebe LIGAR ou DESLIGAR |

## Monitorar via browser

1. Acesse [hivemq.com/demos/websocket-client](https://www.hivemq.com/demos/websocket-client/)
2. Clique em **Connect**
3. Em **Topic** digite `mackenzie/ilhas-calor/#`
4. Clique em **Subscribe**

## Autores

- Vitoria Barbosa
- Luiz Ferreira
- Andre Luis de Oliveira

Universidade Presbiteriana Mackenzie — 2026