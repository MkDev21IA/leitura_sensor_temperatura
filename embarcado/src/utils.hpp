/**
 * @file utils.hpp
 * @brief Funções utilitárias para conversão e registro de dados.
 *
 * Este arquivo define funções auxiliares usadas em diversas partes do projeto,
 * incluindo conversão de leituras analógicas (ADC) para temperatura em Celsius
 * e geração de logs simples no console.
 */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

/**
 * @brief Converte o valor bruto do ADC em temperatura (°C).
 *
 * Aplica uma equação de conversão ou modelo (por exemplo, Steinhart-Hart)
 * para transformar o valor lido pelo ADC em temperatura.
 * A implementação deve considerar as constantes do circuito (resistência série,
 * coeficientes do termistor, etc.).
 *
 * @param rawValue Valor bruto lido do conversor ADC.
 * @return Temperatura correspondente em graus Celsius.
 */
float convertRawToCelsius(int rawValue);

/**
 * @brief Exibe uma mensagem de log no console.
 *
 * Função simples para registrar eventos, erros ou mensagens informativas
 * durante a execução do programa.
 *
 * Exemplo de uso:
 * @code
 * logData("Iniciando leitura do sensor...");
 * @endcode
 *
 * @param msg Mensagem de texto a ser exibida no log.
 */
void logData(const std::string &msg);

#endif // UTILS_HPP
