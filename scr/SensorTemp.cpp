/**
 * @file ADCReader.h
 * @brief Classe para leitura de ADC e medição de temperatura via termistor em sistema Linux embarcado STM32.
 */

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>

/**
 * @class ADCReader
 * @brief Classe responsável pela leitura de ADC e conversão para temperatura via termistor.
 *
 * Esta classe encapsula todas as funcionalidades de leitura ADC, conversão de tensão
 * e cálculo de temperatura usando termistor NTC com equação de Steinhart-Hart.
 */
class ADCReader {
private:
    std::string adcFilePath; ///< Caminho para o arquivo ADC no sistema
    int adcMaxValue; ///< Valor máximo do ADC
    float vRef; ///< Tensão de referência do ADC

    // Constantes do termistor
    double seriesResistor; ///< Resistência em série (Ohms)
    double steinhartA; ///< Coeficiente A de Steinhart-Hart
    double steinhartB; ///< Coeficiente B de Steinhart-Hart
    double steinhartC; ///< Coeficiente C de Steinhart-Hart

public:
    /**
     * @brief Construtor da classe ADCReader.
     *
     * @param filePath Caminho para o arquivo ADC (padrão: canal 13)
     * @param maxValue Valor máximo do ADC (padrão: 16384 para 14 bits)
     * @param referenceVoltage Tensão de referência (padrão: 3.3V)
     */
    ADCReader(const std::string& filePath = "/sys/bus/iio/devices/iio:device0/in_voltage13_raw",
        int maxValue = 16384,
        float referenceVoltage = 3.3f)
        : adcFilePath(filePath)
        , adcMaxValue(maxValue)
        , vRef(referenceVoltage)
        , seriesResistor(10000.0)
        , steinhartA(0.001129148)
        , steinhartB(0.000234125)
        , steinhartC(0.0000000876741)
    {
    }

    /**
     * @brief Define os coeficientes de Steinhart-Hart para o termistor.
     *
     * @param A Coeficiente A
     * @param B Coeficiente B
     * @param C Coeficiente C
     * @param resistance Resistência em série (Ohms)
     */
    void setThermistorParameters(double A, double B, double C, double resistance = 10000.0)
    {
        steinhartA = A;
        steinhartB = B;
        steinhartC = C;
        seriesResistor = resistance;
    }

    /**
     * @brief Configura o caminho do arquivo ADC.
     *
     * @param filePath Novo caminho para o arquivo ADC
     */
    void setADCFilePath(const std::string& filePath)
    {
        adcFilePath = filePath;
    }

    /**
     * @brief Lê o valor bruto (RAW) do ADC.
     *
     * @return Valor RAW do ADC (inteiro >= 0) ou -1 em caso de falha
     */
    int readRawADC()
    {
        std::ifstream adcFile(adcFilePath);
        int value;

        if (adcFile.is_open()) {
            adcFile >> value;
            adcFile.close();
            return value;
        } else {
            std::cerr << "Erro ao abrir o arquivo do ADC: " << adcFilePath << std::endl;
            return -1;
        }
    }

    /**
     * @brief Converte valor RAW do ADC para tensão.
     *
     * @param rawValue Valor bruto do ADC
     * @return Tensão correspondente em Volts
     */
    float convertToVoltage(int rawValue)
    {
        if (rawValue < 0)
            return -1.0f;
        return (float)rawValue * vRef / adcMaxValue;
    }

    /**
     * @brief Calcula a temperatura usando termistor e equação de Steinhart-Hart.
     *
     * @param rawADC Valor bruto lido do ADC
     * @return Temperatura calculada em graus Celsius
     */
    double calculateTemperature(int rawADC)
    {
        if (rawADC <= 0)
            return -273.15;

        double resistance = seriesResistor * ((adcMaxValue / (double)rawADC) - 1.0);
        double logR = log(resistance);
        double tempK = 1.0 / (steinhartA + steinhartB * logR + steinhartC * logR * logR * logR);

        return tempK - 273.15;
    }

    /**
     * @brief Realiza uma leitura completa: ADC, tensão e temperatura.
     *
     * @param rawValue Referência para armazenar valor bruto
     * @param voltage Referência para armazenar tensão
     * @param temperature Referência para armazenar temperatura
     * @return true se a leitura foi bem-sucedida, false caso contrário
     */
    bool readAll(int& rawValue, float& voltage, double& temperature)
    {
        rawValue = readRawADC();

        if (rawValue >= 0) {
            voltage = convertToVoltage(rawValue);
            temperature = calculateTemperature(rawValue);
            return true;
        }

        return false;
    }

    /**
     * @brief Imprime os valores lidos na saída padrão.
     *
     * @param showTemperature Se deve mostrar a temperatura calculada
     */
    void printReading(bool showTemperature = true)
    {
        int raw;
        float voltage;
        double temperature;

        if (readAll(raw, voltage, temperature)) {
            std::cout << "Leitura ADC RAW: " << raw
                      << " --- Tensão: " << voltage << "V";

            if (showTemperature) {
                std::cout << " --- Temperatura: " << temperature << "°C";
            }

            std::cout << std::endl;
        } else {
            std::cerr << "Falha na leitura ADC" << std::endl;
        }
    }

    /**
     * @brief Inicia loop contínuo de leituras.
     *
     * @param intervalSeconds Intervalo entre leituras em segundos
     * @param showTemperature Se deve mostrar a temperatura
     */
    void startContinuousReading(int intervalSeconds = 1, bool showTemperature = true)
    {
        while (true) {
            printReading(showTemperature);
            sleep(intervalSeconds);
        }
    }

    // Getters
    int getADCMaxValue() const { return adcMaxValue; }
    float getVRef() const { return vRef; }
    std::string getADCFilePath() const { return adcFilePath; }
};

/**
 * @brief Exemplo de uso da classe ADCReader.
 */
int main()
{
    // Criar instância da classe ADC
    ADCReader adc;

    // Opcional: configurar parâmetros específicos do termistor
    // adc.setThermistorParameters(0.001129148, 0.000234125, 0.0000000876741, 10000.0);

    // Opcional: configurar canal ADC diferente
    // adc.setADCFilePath("/sys/bus/iio/devices/iio:device0/in_voltage12_raw");

    // Iniciar leituras contínuas (com temperatura)
    adc.startContinuousReading(1, true);

    /* Exemplo de uso alternativo - leituras individuais:
     *
     *   int raw;
     *   float voltage;
     *   double temperature;
     *
     *   if (adc.readAll(raw, voltage, temperature)) {
     *       std::cout << "RAW: " << raw << ", Tensão: " << voltage
     *                 << "V, Temperatura: " << temperature << "°C" << std::endl;
}

*/

    return 0;
}
