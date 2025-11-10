/**
 * @file sensor.hpp
 * @brief Declaração da classe Sensor responsável pela leitura e conversão de dados analógicos.
 *
 * Esta classe representa um sensor analógico (como um termistor NTC),
 * permitindo ler o valor bruto do ADC e convertê-lo em uma grandeza física,
 * como temperatura, utilizando o modelo de Steinhart-Hart.
 */

#ifndef SENSOR_HPP
#define SENSOR_HPP

#include <string>

/**
 * @class Sensor
 * @brief Classe para leitura e conversão de valores de um sensor analógico.
 *
 * A classe encapsula a lógica de leitura de valores analógicos e a conversão
 * para unidades físicas, utilizando as constantes do modelo de Steinhart-Hart.
 */
class Sensor {
private:
    float lastValue;      /**< Último valor lido e convertido do sensor. */
    std::string unit;     /**< Unidade de medida do sensor (ex: "°C"). */

    /**
     * @name Constantes de calibração
     * @brief Parâmetros utilizados para conversão do valor do ADC em temperatura.
     * @{
     */
    float seriesResistor = 10000.0f;      /**< Resistência em série com o sensor (ohms). */
    float adcMaxValue = 65535.0f;          /**< Valor máximo do ADC (resolução de 12 bits). */
    float steinhartA = 1.009249522e-03f;  /**< Coeficiente A da equação de Steinhart-Hart. */
    float steinhartB = 2.378405444e-04f;  /**< Coeficiente B da equação de Steinhart-Hart. */
    float steinhartC = 2.019202697e-07f;  /**< Coeficiente C da equação de Steinhart-Hart. */
    /** @} */

    public:
        /**
         * @brief Construtor padrão da classe Sensor.
         *
         * Inicializa o sensor e define valores padrão para as constantes de calibração.
         */
        Sensor();

        /**
         * @brief Destrutor da classe Sensor.
         *
         * Libera recursos, se necessário.
         */
        ~Sensor();

        /**
         * @brief Lê o valor bruto do sensor (ADC).
         *
         * @return Valor inteiro correspondente à leitura do ADC.
         */
        int readRaw();

        /**
         * @brief Lê e converte o valor analógico para a unidade física configurada.
         *
         * Realiza a leitura do ADC, calcula a resistência equivalente e aplica
         * a equação de Steinhart-Hart para obter a temperatura (ou outra grandeza).
         *
         * @return Valor convertido em unidade física (ex: temperatura em °C).
         */
        float readValue();

        /**
         * @brief Retorna a unidade de medida atual do sensor.
         *
         * @return Unidade de medida como string (ex: "°C", "V").
         */
        std::string getUnit();
};

#endif // SENSOR_HPP
