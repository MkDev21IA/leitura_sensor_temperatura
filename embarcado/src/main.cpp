/**
 * @file main.cpp
 * @brief Programa principal para leitura de sensor e envio periódico de dados via UDP.
 *
 * Este módulo inicializa a comunicação UDP, realiza leituras periódicas de temperatura
 * e envia as informações formatadas em JSON para um servidor remoto.
 *
 * O ciclo principal executa continuamente:
 *  - Lê a temperatura do sensor (via função `getTemperature()`).
 *  - Monta um pacote `UdpPacket` com metadados e timestamp.
 *  - Serializa o pacote em JSON.
 *  - Envia o JSON via `UDPClient` a cada 1 segundo.
 */

#include "../include/utils.hpp"
#include "../include/udp_client.hpp"
#include "../include/udp_protocol.hpp"
#include <iostream>
#include <thread>


/**
 * @brief Função externa que lê a temperatura do sensor embarcado.
 *
 * Esta função é implementada em outro módulo (`main_embarcado.cpp`)
 * e fornece o valor da temperatura em graus Celsius.
 *
 * @return Temperatura medida em °C.
 */
extern float getTemperature();

/**
 * @brief Função principal da aplicação.
 *
 * Inicializa o cliente UDP e executa um loop contínuo que:
 * 1. Lê a temperatura do sensor.
 * 2. Cria um pacote `UdpPacket` com as informações do sensor.
 * 3. Serializa o pacote em JSON.
 * 4. Envia os dados ao servidor via UDP.
 *
 * Em caso de falha no envio, é exibida uma mensagem de erro no console.
 * O envio é repetido a cada 1 segundo.
 *
 * @return Código de status do programa (0 = sucesso).
 */
int main() {
    logData("Iniciando comunicação UDP...");
    UDPClient client(" 192.168.42.10", 5000); /**< Cliente UDP para envio de pacotes. */

    while (true) {
        float valor = getTemperature();

        // Monta o pacote JSON
        UdpPacket pkt;
        pkt.group_id  = "grupo6";
        pkt.sensor_id = "SensorDeTemperatura";
        pkt.value     = valor;
        pkt.unit      = "°C";
        pkt.timestamp = currentTimestamp();

        std::string jsonData = serialize(pkt);

        // Envia o JSON pelo UDP
        if (client.sendData(jsonData)) {
            std::cout << "Enviado: " << jsonData << std::endl;
        } else {
            std::cerr << "Erro ao enviar pacote UDP!" << std::endl;
        }

        // Aguarda 1 segundo antes da próxima leitura
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
