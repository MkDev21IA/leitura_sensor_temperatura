# 🌡️ Leitura de Sensor de Temperatura (KY-013) com STM32MP1

![Placa](https://img.shields.io/badge/Placa-STM32MP1%20DK1-blue)
![Linguagem](https://img.shields.io/badge/Linguagem-C%2B%2B-blue.svg)
![Disciplina](https://img.shields.io/badge/Disciplina-Programação%20Aplicada-lightgrey)

Este repositório contém o código-fonte e a documentação para um projeto de Desenvolvimento Embarcado. O objetivo principal é demonstrar a comunicação e a leitura de dados de um **sensor de temperatura analógico KY-013** utilizando o kit de desenvolvimento **STM32MP1 DK1**.

---

## 👥 Equipe

-   **Mikhael Silveira** – Engenharia Eletrônica
-   **Rafael Sodré** – Engenharia Elétrica
-   **Marcos Vinícius** – Engenharia de Comunicações

---

## 1. Visão Geral do Projeto

O projeto foca na integração de hardware e software para adquirir dados de temperatura do ambiente. Este é um exemplo prático que ilustra conceitos-chave como:

-   Configuração e uso de periféricos (ADC).
-   Manipulação de dados em tempo real.
-   Exibição de resultados em um console serial.

---

## 2. Pré-requisitos

Para compilar e executar este projeto, você precisará de:

-   Kit de desenvolvimento **STM32MP1 DK1**.
-   **SDK de desenvolvimento ST** com a toolchain de compilação cruzada (`arm-buildroot-linux-gnueabihf`).
-   Conexão com o kit via **USB-C** e acesso por **serial (TeraTerm, PuTTY, etc.)** ou **rede (SSH)**.
-   Programa `scp` (ou similar, como WinSCP) para transferência de arquivos.
-   O código-fonte deste repositório. 

### 1. Compilação

Ambiente de compilação cruzada (cross-toolchain) para ARM, como: `arm-linux-gnueabihf-g++`

### 2. Documentação

-   **Doxygen** para gerar documentação técnica.

 ---
### 3. Organização do Código

O núcleo do projeto é o módulo de leitura e envio de dados de temperatura, abstraído principalmente pela classe `Sensor` e pelos módulos de comunicação UDP. A arquitetura é modular, separando cabeçalhos, implementações e funções utilitárias, facilitando manutenção e testes.

---

### 3.1. Classe Sensor (`sensor.hpp` / `sensor.cpp`)

Responsável por abstrair o hardware do sensor e fornecer leituras convertidas de temperatura.

**Atributos principais**

| Atributo | Tipo | Descrição |
| :--- | :--- | :--- |
| `lastValue` | `float` | Último valor de temperatura lido |
| `unit` | `std::string` | Unidade da leitura (°C) |
| `seriesResistor` | `float` | Resistor em série com o termistor (Ω) |
| `adcMaxValue` | `float` | Valor máximo do ADC (ex: 4095 para 12 bits) |
| `steinhartA`, `steinhartB`, `steinhartC` | `float` | Coeficientes da equação de Steinhart-Hart |

**Funções principais**

| Função | Retorno | Descrição |
| :--- | :--- | :--- |
| `int readRaw()` | `int` | Lê o valor bruto do ADC do arquivo do sistema Linux embarcado (`/sys/bus/iio/devices/iio:device0/in_voltage13_raw`). Retorna -1 em caso de erro. |
| `float readValue()` | `float` | Converte o valor bruto para temperatura em °C usando Steinhart-Hart e atualiza `lastValue`. |
| `std::string getUnit()` | `std::string` | Retorna a unidade da leitura (°C). |

---

### 3.2. Comunicação UDP (`udp_client.hpp` / `udp_client.cpp`)

Responsável por enviar os dados processados para um servidor remoto via protocolo UDP.

**Atributos**

| Atributo | Tipo | Descrição |
| :--- | :--- | :--- |
| `sockfd` | `int` | File descriptor do socket UDP |
| `server_ip` | `std::string` | IP do servidor de destino |
| `server_port` | `int` | Porta UDP do servidor |

**Funções principais**

| Função | Retorno | Descrição |
| :--- | :--- | :--- |
| `UDPClient(const std::string &ip, int port)` | construtor | Inicializa socket UDP e define destino |
| `bool sendData(const std::string &data)` | `bool` | Envia dados (JSON) via UDP; retorna `true` se enviado com sucesso |
| `~UDPClient()` | destruidor | Fecha o socket UDP |

---

### 3.3. Formatação de Pacotes (`data_formatter.hpp` / `data_formatter.cpp`)

Módulo responsável por preparar e formatar os dados em pacotes padronizados (`UdpPacket`) para envio.

**Funções principais**

| Função | Retorno | Descrição |
| :--- | :--- | :--- |
| `UdpPacket preparePacket(const std::string &group, const std::string &sensor, double value, const std::string &unit)` | `UdpPacket` | Cria e retorna um pacote UDP completo, preenchendo `group_id`, `sensor_id`, `value`, `unit` e `timestamp` atual |

---

### 3.4. Estrutura do Pacote UDP (`udp_protocol.hpp`)

Define o formato dos pacotes enviados e funções auxiliares para serialização.

**Struct principal**

```cpp
struct UdpPacket {
    std::string group_id;
    std::string sensor_id;
    double value;
    std::string unit;
    std::string timestamp;
};
```

**Funções auxiliares**

| Função | Retorno | Descrição |
| :--- | :--- | :--- |
| `std::string currentTimestamp()` | `string` | Gera `timestamp` atual em UTC no formato ISO8601 |
| `std::string serialize(const UdpPacket &p)` | `string` | Converte `UdpPacket` em JSON simples |

---

### 3.5. Funções utilitárias (`utils.hpp` / `utils.cpp`)

Contém funções auxiliares usadas pelo sensor e pelo sistema de logging.

| Função | Retorno | Descrição |
| :--- | :--- | :--- |
| `float convertRawToCelsius(int rawValue)` | `float` | Converte valor bruto do ADC em temperatura (°C) usando Steinhart-Hart |
| `void logData(const std::string &msg)` | `void` | Escreve mensagens de log no console com prefixo `[LOG]` |

---

### 3.6. Programa principal (`main.cpp`)

Instancia `Sensor` e `UDPClient`.

**Loop contínuo:**

1.  Lê temperatura do sensor (`Sensor::readValue()`)
2.  Prepara pacote JSON (`DataFormatter::preparePacket`)
3.  Envia via UDP (`UDPClient::sendData`)
4.  Mostra informações no console

Intervalo padrão: 1 segundo

Interrupção: Ctrl+C

---

### ✅ Resumo da Arquitetura

```
Sensor (leitura ADC) ---> DataFormatter (prepara pacote)
                                 |
                                 v
                             UDPClient (envia pacote)
                                 |
                                 v
                           Servidor remoto (recebe dados)
```

Essa estrutura modular facilita:

-   Testes unitários (em `test/`)
-   Substituição do sensor ou do protocolo de envio
-   Manutenção e expansão do projeto


 ---

## 4. Estrutura do Repositório
```
├── 📁 embarcado/                 # Parte 1 - Desenvolvimento embarcado
│   ├── include/                  # Cabeçalhos (.h)
│   │   ├── sensor.hpp            # Classe do sensor
│   │   └── utils.hpp             # Funções auxiliares
│   │   ├── udp_client.hpp
│   │   ├── udp_protocol.hpp      # Estrutura dos pacotes, formatação JSON/CSV
│   │   └── data_formatter.hpp
│   ├── src/                      # Implementações (.cpp)
│   │   ├── sensor.cpp
│   │   └── main_embarcado.cpp    
│   │   ├── udp_client.cpp
│   │   ├── data_formatter.cpp
│   │   └── main.cpp              # Programa principal no kit
│   ├── test/                     # Testes unitários (se aplicável)
│   └── README.md                 # Instruções específicas da parte embarcada
├── 📁 servidor_gui/              # Parte 3 - Interface gráfica
├── 📁 build/      # Arquivos binários gerados pela compilação
├── 📁 html       # Documentação 
├── 📁 latex       # Imagens e screenshots do projeto
│   README.md   # Documentação principal
└── imagem_sensor.jpg
└── imagem_sensor_udp.jpg
```

---

## 5. Montagem do Hardware

A seguir, a pinagem para a conexão do sensor KY-013 ao kit STM32MP1 DK1.

⚠️ **Atenção:** Certifique-se de que a placa esteja **desligada** antes de fazer as conexões.

| Pino do KY-013 | Função | Pino do STM32MP1 DK1 (Header Arduino) |
| :--- | :--- | :--- |
| **S** | Sinal Analógico | `ADC_INx` (Pino Analógico) |
| **VCC** (Meio) | +3.3V | `3.3V` |
| **GND** | Terra | `GND` |

**Nota Importante:** O pino `ADC_INx` é genérico. Você deve verificar no arquivo `embarcado/src/sensor.cpp` qual pino ADC específico (ex: `A0`, `A1`, etc.) foi configurado no código para conectar o pino de **Sinal (S)** do sensor.

---

## 6. Instruções de Compilação e Execução

O processo utiliza **compilação cruzada** (cross-compilation) para gerar um binário executável para a arquitetura ARM do STM32MP1.

### 6.1. Configuração do Ambiente (Toolchain)

Antes de compilar, você precisa ter o SDK da ST instalado e a toolchain de compilação cruzada acessível em seu terminal.

1.  **Extraia o SDK** (caso ainda não o tenha feito):
    ```bash
    tar -xvf arm-buildroot-linux-gnueabihf_sdk-DK2.tar.gz
    ```

2.  **Adicione a Toolchain ao seu PATH:**
    Adicione o diretório `bin` do SDK ao seu `PATH` de sistema. Isso permite que você chame o compilador `arm-linux-g++` de qualquer lugar.

    ```bash
    # Adicione esta linha ao seu ~/.bashrc ou ~/.zshrc
    export PATH=$PATH:/caminho/completo/para/arm-buildroot-linux-gnueabihf_sdk-DK2/bin
    ```
    *Substitua `/caminho/completo/para/` pelo local onde você extraiu o SDK.*

3.  **Verifique a instalação:**
    Abra um novo terminal e digite:
    ```bash
    arm-linux-g++ --version
    ```
    Se ele mostrar a versão do compilador, você está pronto.

### 6.2. Compilação e Execução

1.  **Clone este repositório** e entre na pasta do projeto.

2.  **Compile o programa:**
    Execute o comando a seguir na raiz do projeto. Ele compilará o `sensor.cpp` e salvará o executável final na pasta `build/`.
    ```bash
    arm-linux-g++ embarcado/src/sensor.cpp -o build/sensor
    ```

3.  **Envie o binário para a placa:**
    Use `scp` para transferir o arquivo. O IP padrão da placa via conexão USB-C costuma ser `192.168.42.2`.
    ```bash
    # O -O é necessário em versões mais recentes do scp
    scp -O build/sensor root@<ip_do_kit>:/home/root/
    ```

4.  **Execute na Placa:**
    Acesse o terminal da sua placa (via SSH ou serial) e execute os seguintes comandos:
    ```bash
    # Dê permissão de execução ao arquivo
    chmod +x /home/root/sensor

    # Rode o programa
    ./home/root/sensor
    ```

    Você deverá ver as leituras de temperatura aparecendo no console.

---

## 7. Leitura dos dados do sensor pelo terminal serial

A imagem abaixo mostra o programa em execução, exibindo as leituras de temperatura no terminal serial.

![Leitura da Temperatura](imagem_sensor.jpg)

---

## 8. Leitura do sensor via UDP

A imagem abaixo mostra o programa em execução, realizando as leituras do ADC, processando e enviando os resultados em um pacote json via UDP.

![Leitura da Temperatura](imagem_sensor_udp.jpg)
