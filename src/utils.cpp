/**
 * @file utils.cpp
 * @brief Implementação de funções utilitárias para conversão de ADC e log no console.
 *
 * Contém funções auxiliares para:
 *  - Converter leituras brutas do ADC em temperatura (°C) usando o modelo Steinhart-Hart.
 *  - Registrar mensagens de log simples no console.
 */

#include "utils.hpp"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>

/**
 * @brief Converte um valor bruto do ADC em temperatura em °C.
 *
 * Aplica o modelo de Steinhart-Hart para um termistor NTC.
 * Se o valor do ADC for inválido (≤ 0), retorna -273.15°C como valor de erro.
 *
 * @param rawADC Valor lido do ADC (inteiro).
 * @return Temperatura correspondente em graus Celsius.
 */
float convertRawToCelsius(int rawADC)
{
    if (rawADC <= 0)
        return -273.15f; // retorna valor mínimo possível (erro)

        // Constantes do circuito e do termistor
        const float seriesResistor = 10000.0f;   /**< Resistor fixo em série (Ω). */
        const float adcMaxValue = 4095.0f;       /**< Valor máximo do ADC (12 bits). */
        const float steinhartA = 1.009249522e-03f; /**< Coeficiente A do Steinhart-Hart. */
        const float steinhartB = 2.378405444e-04f; /**< Coeficiente B do Steinhart-Hart. */
        const float steinhartC = 2.019202697e-07f; /**< Coeficiente C do Steinhart-Hart. */

        // Cálculo de Steinhart-Hart
        float resistance = seriesResistor * ((adcMaxValue / (float)rawADC) - 1.0f);
        float logR = logf(resistance);
        float tempK = 1.0f / (steinhartA + steinhartB * logR + steinhartC * logR * logR * logR);

        // Converte Kelvin para Celsius
        return tempK - 273.15f;
}

/**
 * @brief Exibe uma mensagem de log simples no console.
 *
 * Prefixa a mensagem com "[LOG]" e envia para std::cout.
 *
 * @param msg Mensagem de texto a ser exibida.
 */
void logData(const std::string &msg) {
    std::cout << "[LOG] " << msg << std::endl;
}



