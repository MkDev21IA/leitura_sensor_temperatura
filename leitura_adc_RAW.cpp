#include <cmath>
#include <fstream>
#include <iostream>
#include <unistd.h>

#define ADC_RAW_FILE "/sys/bus/iio/devices/iio:device0/in_voltage13_raw"
#define ADC_MAX_VALUE 16384 // STM32 ADC 14 bits
#define VREF 3.3 // Tensão de referência (VCC)

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

double Thermistor(int RawADC)
{
    // Dados do circuito
    const double SERIES_RESISTOR = 10000.0; // 10kΩ em série com o termistor
    const double ADC_MAX = 16384; // STM32 ADC 14 bits
    // Coeficientes do Steinhart-Hart — validado conforme o modelo do NTC
    const double A = 0.001129148;
    const double B = 0.000234125;
    const double C = 0.0000000876741;

    // Proteção contra divisão por zero:
    if (RawADC == 0)
        return -273.15;

    // Calcular resistência do termistor a partir do valor ADC
    double resistance = SERIES_RESISTOR * ((ADC_MAX / RawADC) - 1.0);
    double logR = log(resistance);

    // Equação de Steinhart-Hart
    double tempK = 1.0 / (A + B * logR + C * logR * logR * logR);

    // Kelvin para Celsius
    return tempK - 273.15;
}

int main()
{
    while (true) {
        int raw = readADC();
        if (raw >= 0) {
            float voltage = (float)raw * VREF / ADC_MAX_VALUE;
            std::cout << "Leitura ADC RAW: " << raw << " --- Tensão: " << voltage << std::endl;
        } else {
            std::cerr << "Falha na leitura ADC" << std::endl;
        }
        sleep(1); // Espera 1 segundo antes da próxima leitura
    }
    return 0;
}
