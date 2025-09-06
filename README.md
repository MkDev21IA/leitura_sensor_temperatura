# 🌡️ Monitoramento Remoto – Sensor de Temperatura Analógica

Sistema embarcado para **monitoramento remoto de condições térmicas**, desenvolvido com o **kit STM32MP1-DK1** e o sensor analógico **KY-013**, com transmissão contínua via **UDP** e supervisão em **interface gráfica multiplataforma**.

---

## 🚀 Visão Geral
Cargas sensíveis exigem **rastreabilidade e monitoramento constante**. Este projeto implementa um sistema embarcado que:
- Lê a temperatura em tempo real pelo **ADC do STM32MP1-DK1**  
- Converte o valor bruto para **graus Celsius**  
- Transmite os dados via **UDP** para um servidor na rede local  
- Exibe as leituras em uma **interface gráfica no PC**, com histórico e alertas configuráveis  

O sistema segue uma arquitetura **IoT modular**, permitindo integração futura com outros sensores (umidade, luminosidade, vibração etc.).

---

## 👥 Equipe
- **Mikhael Silveira** – Engenharia Eletrônica  
- **Rafael Sodré** – Engenharia Elétrica  
- **Marcos Vinícius** – Engenharia de Comunicações  

---

## 🛠️ Funcionalidades
- 📡 **Aquisição de dados** via ADC (IIO Linux)  
- 🔄 **Conversão** para °C com calibração linear  
- 📤 **Transmissão UDP** em tempo real  
- 🖥️ **Interface gráfica** com:
  - Valor atual da temperatura  
  - Histórico gráfico (últimos 60s)  
  - Alertas visuais para valores fora da faixa segura  
  - Exportação para CSV com timestamps  

---

## ⚙️ Arquitetura
```
[KY-013] → [ADC STM32MP1] → [Classe C++ SensorTemp]
   ↓                               ↓
   └──────────────→ UDP Socket → Servidor PC → Interface Gráfica
```

- **Embarcado (C++)**: leitura do ADC, conversão e envio UDP  
- **Servidor/PC (Python/Qt)**: recepção, processamento e visualização  

---

## 📂 Estrutura do Repositório
```
├── src/                # Código embarcado (C++)
│   ├── SensorTemp.cpp  # Classe do sensor
│   └── SensorTemp.h
├── gui/                # Interface gráfica (Python/Qt + Matplotlib)
│   └── monitor.py
├── docs/               # Documentação e relatórios
│   ├── diagramas/
│   └── relatorio.pdf
├── build/              # Binários compilados
├── Makefile            # Automação da compilação cruzada
└── README.md           # Este arquivo
```

---

## 🔧 Instalação e Execução

### 1. Compilação cruzada (C++ embarcado)
Na VM/Linux com toolchain configurada:
```bash
$GXXCOMPILER ./scr/SensorTemp.cpp -o ./build/sensor
```
O binário será gerado em `build/`.

### 2. Deploy no kit
```bash
scp build/sensor root@<ip_da_placa>:/home/root
```

### 3. Execução no kit
```bash
ssh root@<ip_da_placa>
./sensor
```

### 4. Execução da interface no PC
```bash
cd gui
python3 monitor.py
```

---

## 📡 Protocolo de Comunicação
Formato das mensagens enviadas via UDP:
```
<SENSOR_ID>,<VALOR>,<UNIDADE>,<TIMESTAMP>
```

Exemplo:
```
TEMP01,25.3,C,2025-09-06T18:30:25
```

---

## 📚 Tecnologias Utilizadas
- **C++17** – leitura e transmissão no embarcado  
- **Linux IIO Subsystem** – acesso ao ADC  
- **UDP Socket** – comunicação em tempo real  
- **Python 3 + PyQt5 + Matplotlib** – interface gráfica  
- **Doxygen** – documentação do código  

---

## 🚧 Melhorias Futuras
- 🔧 Ajuste dinâmico de limites de alerta via interface  
- 📊 Relatórios automáticos em PDF/CSV  
- 🌍 Integração com servidor em nuvem (IoT)  
- 🔋 Monitoramento de energia e estado da bateria  

---

## 📸 Demonstração
*(incluir imagens/gifs da interface e da montagem do hardware quando disponíveis)*  
