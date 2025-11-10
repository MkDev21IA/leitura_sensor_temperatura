/**
 * @file sensor.cpp
 * @brief Implementação da classe Sensor para leitura e conversão de dados analógicos.
 *
 * Este módulo fornece a lógica para ler valores brutos de um sensor analógico
 * via arquivo do sistema (ADC) e convertê-los para unidades físicas, como
 * temperatura em °C, utilizando funções auxiliares de conversão.
 */

#include "../include/sensor.hpp"
#include "../include/utils.hpp"
#include <iostream>
#include <fstream>

/**
 * @brief Construtor da classe Sensor.
 *
 * Inicializa o último valor lido como 0 e define a unidade padrão como "°C".
 */
Sensor::Sensor() {
    lastValue = 0.0f;
    unit = "°C";
}

/**
 * @brief Destrutor da classe Sensor.
 *
 * Atualmente não realiza operações adicionais.
 */
Sensor::~Sensor() {}

/**
 * @brief Lê o valor bruto do sensor (ADC) a partir de arquivo do sistema.
 *"/sys/bus/iio/devices/iio:device0/in_voltage13_raw"
 * O valor é lido do arquivo correspondente ao canal ADC utilizado
 * pelo sensor embarcado.
 *
 * @return Valor inteiro lido do ADC. Retorna -1 em caso de erro ao abrir ou ler o arquivo.
 */
int Sensor::readRaw() {
    const std::string path = "/sys/bus/iio/devices/iio:device0/in_voltage13_raw";
    std::ifstream file(path);

    if (!file.is_open()) {
        std::cerr << "Erro: não foi possível abrir " << path << std::endl;
        return -1;  /**< Valor negativo indica falha na leitura. */
    }

    int rawValue = 0;
    file >> rawValue;

    if (file.fail()) {
        std::cerr << "Erro: falha ao ler valor de " << path << std::endl;
        return -1;
    }

    file.close();
    return rawValue;
}

/**
 * @brief Lê e converte o valor do sensor para unidade física (°C).
 *
 * Chama `readRaw()` para obter o valor do ADC e depois utiliza
 * a função `convertRawToCelsius()` para converter para temperatura.
 * O último valor lido é armazenado em `lastValue`.
 *
 * @return Valor convertido em °C.
 */
float Sensor::readValue() {
    int raw = readRaw();
    lastValue = convertRawToCelsius(raw);
    return lastValue;
}

/**
 * @brief Retorna a unidade de medida do sensor.
 *
 * @return Unidade como string (ex: "°C").
 */
std::string Sensor::getUnit() {
    return unit;
}
