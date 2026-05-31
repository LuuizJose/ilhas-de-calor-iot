# Interfaces, Protocolos e Comunicação

## 🏗️ Arquitetura do Sistema
O sistema utiliza uma arquitetura IoT estruturada em torno do protocolo MQTT, operando por meio de dois fluxos principais de dados:

* **Fluxo de Telemetria (Envio):** `DHT22` ──► `ESP32` ──► `Broker MQTT` ──► `MQTT Explorer`
* **Fluxo de Controle (Recebimento):** `MQTT Explorer` ──► `Broker MQTT` ──► `ESP32` ──► `Atuador (LED/Bomba)`

---

## 🔌 Interfaces de Hardware e Conectividade

### Interface GPIO
A pinagem do microcontrolador ESP32 foi mapeada de forma dedicada para garantir a integridade dos sinais lógicos:
* **GPIO 4:** Configurada como entrada digital para recepção do barramento de dados (*one-wire*) do sensor **DHT22**.
* **GPIO 5:** Configurada como saída digital (*GPO*) para o controle de acionamento do atuador de resfriamento.

### Interface Wi-Fi
O módulo de rádio interno do ESP32 é inicializado para estabelecer a conectividade com a internet. No ambiente de testes, a interface adota as seguintes credenciais:
* **SSID:** `Wokwi-GUEST`
* **Segurança:** Autenticação aberta (perfil padrão do simulador Wokwi)

---

## 🌐 Protocolo de Comunicação MQTT
O **MQTT (Message Queuing Telemetry Transport)** foi selecionado por ser um protocolo leve, de baixa sobrecarga de rede (*overhead*) e baseado no modelo de **Publicação/Assinatura (Publish/Subscribe)**.

### Infraestrutura do Broker
* **Broker Host:** `broker.hivemq.com` (Servidor público em nuvem da HiveMQ)
* **Porta:** `1883` (Comunicação TCP padrão sem criptografia TLS)

### 📌 Arquitetura de Tópicos

| Contexto | Função de Dados | Caminho do Tópico MQTT |
| :--- | :--- | :--- |
| **Publicação** | Telemetria de Temperatura | `mackenzie/ilhas-calor/temperatura` |
| **Publicação** | Telemetria de Umidade | `mackenzie/ilhas-calor/umidade` |
| **Publicação** | Estado Atual da Bomba | `mackenzie/ilhas-calor/bomba` |


### 🕹️ Dicionário de Comandos (Payload)
As mensagens recebidas no tópico de subscrição são processadas por uma rotina de comparação de *strings*. O firmware responde aos seguintes comandos estritos:
* `LIGAR`: Comuta o estado da GPIO 5 para nível lógico alto (High), ativando o circuito de resfriamento.
* `DESLIGAR`: Comuta o estado da GPIO 5 para nível lógico baixo (Low), cessando a mitigação térmica.

---

## 🔄 Dinâmica de Operação
* **Operação de Borda:** O ESP32 realiza a leitura física das variáveis climáticas e publica autonomamente as informações nos respectivos tópicos de telemetria.
* **Intervenção Remota:** Utilizando o **MQTT Explorer**, o usuário consegue monitorar os gráficos de atualização em tempo real e injetar os *payloads* de comando no broker, que propaga imediatamente a instrução até a borda do sistema.

---

## 📊 Análise de Desempenho
As medições de tempo de tráfego de mensagens revelaram os seguintes índices de latência:

* **Tempo Médio (Sensor ──► MQTT):** 3,0 ms
* **Tempo Médio (Comando ──► Atuador):** 1,5 ms

Os dados obtidos em ambiente controlado confirmam uma excelente eficiência na entrega de pacotes. Essa temporização de baixa latência valida a viabilidade técnica da arquitetura para cenários críticos de automação e resposta rápida a emergências microclimáticas urbanas.