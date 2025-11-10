/**
 * @file main_embarcado.cpp
 * @brief Implementação da função para leitura de temperatura do sensor embarcado.
 *
 * Este módulo define a função `getTemperature()`, utilizada pelo main principal
 * (`main.cpp`) para obter leituras periódicas de temperatura.
 * A função encapsula a criação e reutilização do objeto `Sensor`.
 */

#include "../include/sensor.hpp"

/**
 * @brief Lê a temperatura do sensor embarcado.
 *
 * Utiliza um objeto estático da classe `Sensor` para garantir que
 * a inicialização do sensor ocorra apenas uma vez. A função
 * retorna o valor convertido em graus Celsius.
 *
 * @return Temperatura medida pelo sensor em °C.
 */
float getTemperature() {
    static Sensor tempSensor; /**< Objeto estático que representa o sensor. */

    return tempSensor.readValue(); /**< Retorna o valor convertido da leitura ADC. */
}
