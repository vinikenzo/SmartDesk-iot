# Projeto SmartDesk – IoT MQTT

## Integrantes
| Integrante | RM |
|-----------|------|
| João Victor Alves da Silva | RM559726 |
| Vinicius Kenzo Tocuyosi | RM559982 |

---

## 1. Definição do Projeto

### Objetivo do Projeto
O **SmartDesk** é um sistema de gerenciamento inteligente de espaços de trabalho.  
Seu objetivo principal é **otimizar a utilização de mesas e ambientes corporativos**, permitindo que empresas, coworkings e equipes acompanhem:

- Quais mesas estão disponíveis ou ocupadas  
- Horários de uso e padrões de ocupação  
- Informações em tempo real para melhorar organização e produtividade  

A solução utiliza IOT para captar o estado das mesas, e o foco é **entregar um ambiente mais eficiente, organizado e com melhor experiência de uso**.

### Escopo

**Hardware / Simulação**
- ESP32 rodando no Wokwi  
- Envio de dados MQTT com timestamp via NTP  
- Simulação de ocupação (0 = livre, 1 = ocupada)

**Gateway**
- Node-RED recebendo e tratando mensagens  
- Fluxos para leitura dos tópicos  
- Dashboard em tempo real

**Dashboard**
- Exibe ocupação das mesas  
- Atualiza automaticamente conforme mensagens MQTT
<img width="1907" height="481" alt="dashboard" src="https://github.com/user-attachments/assets/22310ead-e6a9-483e-9e9e-7538734b6650" />
  

---

## Requisitos

### Requisitos Funcionais
- Enviar dados JSON para o broker MQTT  
- Informações enviadas: `desk_id`, `data`, `hora`, `occupied`, `ip`  
- Processar mensagens no Node-RED  
- Atualizar dashboard em tempo real

### Requisitos Não-Funcionais
- Comunicação em tempo real via MQTT  
- Compatível com Wokwi e Node-RED

---

## 2. Arquitetura da Solução

### Fluxo da Aplicação
1. ESP32 conecta ao Wi-Fi  
2. Obtém data/hora via NTP  
3. Gera JSON para cada mesa  
4. Publica nos tópicos:  
smartdesk/salaA/desk01/sensor  
smartdesk/salaA/desk02/sensor  
smartdesk/salaA/desk03/sensor
5. Node-RED recebe as mensagens  
6. Dashboard exibe status de ocupação  

### Tecnologias Utilizadas
- ESP32  
- Wokwi  
- Broker MQTT (HiveMQ)  
- Node-RED  
- node-red-dashboard  
- ArduinoJson + PubSubClient  

---

## 3. Fluxo no Node-RED

<img width="1360" height="682" alt="FluxoNodered" src="https://github.com/user-attachments/assets/2f12a844-e2cf-457d-a049-7c281744ce19" />

O fluxo Node-RED segue esta estrutura:  
MQTT In (3 tópicos)  

↓ 
JSON (converter texto → objeto)
↓
Switch (filtra desk01, desk02, desk03)  
↓  
Text → Function (formatação)  
↓  
Dashboard (exibição de dados)

Cada mesa tem um card individual exibindo:
- Nome da mesa  
- Status (ocupada / livre)  
- Última atualização  

---

## 4. Como Executar

### No ESP32 (Wokwi)
1. Abrir o projeto no Wokwi  
2. Inserir o código fornecido  
3. Conectar ao broker MQTT  
4. Iniciar a simulação  

### No Node-RED
1. Importar o fluxo do projeto  
2. Instalar **node-red-dashboard**  
3. Configurar o broker MQTT  
4. Executar o fluxo  
5. Abrir o dashboard pelo link do Node-RED  
http://127.0.0.1:1880/ui


---
