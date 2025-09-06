/**
 * @file
 * @brief Leitura de ADC e preparação para medição de temperatura via termistor em sistema Linux embarcado STM32.
 *
 * Este código executa leituras periódicas de um canal ADC e mostra o valor bruto e a tensão correspondente.
 * Também inclui um esqueleto para função de conversão de temperatura com termistor.
 */

#include <cmath>
#include <fstream>
#include <iostream>
#include <unistd.h>

/**
 * @def ADC_RAW_FILE
 * @brief Caminho para o arquivo de leitura do ADC no sistema de arquivos Linux.
 */
#define ADC_RAW_FILE "/sys/bus/iio/devices/iio:device0/in_voltage13_raw"

/**
 * @def ADC_MAX_VALUE
 * @brief Valor máximo de conversão do ADC de 14 bits no STM32.
 */
#define ADC_MAX_VALUE 16384

/**
 * @def VREF
 * @brief Tensão de referência aplicada ao ADC (V).
 */
#define VREF 3.3

/**
 * @brief Lê o valor bruto (RAW) do ADC via arquivo no sistema de arquivos.
 *
 * Abre o arquivo do sistema correspondente ao canal ADC e lê o valor convertido.
 * Retorna o valor inteiro lido ou -1 em caso de erro de acesso ao arquivo.
 *
 * @return Valor RAW do ADC (inteiro >= 0) ou -1 em caso de falha.
 */
int readADC()
{
    std::ifstream adcFile(ADC_RAW_FILE);
    int value;
    if (adcFile.is_open()) {
        adcFile >> value;
        adcFile.close();
        return value;
    } else {
        std::cerr << "Erro ao abrir o arquivo do ADC!" << std::endl;
        return -1;
    }
}

/**
 * @brief Função planejada para calcular a temperatura a partir de um termistor ligado ao ADC.
 *
 * Em futuras implementações, esta função converterá o valor bruto do ADC para uma resistência,
 * e com a equação de Steinhart-Hart determinará a temperatura. Os parâmetros são exemplos
 * para termistores NTC de uso comum, mas devem ser ajustados conforme o sensor real.
 *
 * @param RawADC Valor bruto lido do ADC.
 * @return Temperatura calculada em graus Celsius.
 *
 * @note Esta função pode ser adaptada para uso com outros sensores de temperatura na mesma porta ADC.
 */
double Thermistor(int RawADC)
{
    const double SERIES_RESISTOR = 10000.0; ///< Resistência em série (Ohms)
    const double ADC_MAX = 16384; ///< Faixa máxima do conversor ADC (14 bits)
    const double A = 0.001129148; ///< Coeficiente Steinhart-Hart para exemplo de NTC
    const double B = 0.000234125;
    const double C = 0.0000000876741;

    if (RawADC == 0)
        return -273.15;

    double resistance = SERIES_RESISTOR * ((ADC_MAX / RawADC) - 1.0);
    double logR = log(resistance);

    double tempK = 1.0 / (A + B * logR + C * logR * logR * logR);

    return tempK - 273.15;
}

/**
 * @brief Função principal (loop).
 *
 * Inicializa loop contínuo de leitura do ADC, com pausa de 1 segundo entre leituras. Mostra na tela
 * o valor bruto e a tensão lida, convertendo o valor ADC pela referência configurada.
 * Em caso de leitura inválida, indica erro na saída padrão de erro.
 *
 * @return 0 ao finalizar (caso o loop seja interrompido externamente).
 */
int main()
{
    while (true) {
        int raw = readADC();
        if (raw >= 0) {
            float voltage = (float)raw * VREF / ADC_MAX_VALUE;
            std::cout << "Leitura ADC RAW: " << raw << " --- Tensão: " << voltage << std::endl;
            // Para futura leitura de temperatura, descomente a linha abaixo:
            // std::cout << "Temperatura: " << Thermistor(raw) << " °C" << std::endl;
        } else {
            std::cerr << "Falha na leitura ADC" << std::endl;
        }
        sleep(1);
    }
    return 0;
}
