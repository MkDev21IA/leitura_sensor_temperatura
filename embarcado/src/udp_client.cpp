/**
 * @file udp_client.cpp
 * @brief Implementação da classe UDPClient para envio de dados via protocolo UDP.
 *
 * Este módulo define a criação do socket UDP, envio de pacotes de dados
 * e fechamento do socket quando o cliente é destruído.
 */

#include "udp_client.hpp"
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

/**
 * @brief Construtor da classe UDPClient.
 *
 * Cria um socket UDP e armazena o endereço IP e a porta do servidor destino.
 * Caso ocorra erro na criação do socket, uma mensagem é exibida no console.
 *
 * @param ip Endereço IP do servidor (ex: "127.0.0.1").
 * @param port Porta UDP do servidor.
 */
UDPClient::UDPClient(const std::string &ip, int port)
: server_ip(ip), server_port(port)
{
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
        std::cerr << "Erro criando socket UDP\n";
}

/**
 * @brief Destrutor da classe UDPClient.
 *
 * Fecha o socket UDP, se ele estiver aberto.
 */
UDPClient::~UDPClient() {
    if (sockfd >= 0)
        close(sockfd);
}

/**
 * @brief Envia dados para o servidor via UDP.
 *
 * Configura a estrutura sockaddr_in com IP e porta do servidor,
 * e envia a string de dados usando `sendto()`.
 *
 * @param data String contendo os dados a serem enviados.
 * @return `true` se todos os bytes foram enviados corretamente, `false` em caso de erro.
 */
bool UDPClient::sendData(const std::string &data) {
    if (sockfd < 0)
        return false;

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(server_port);
    if (inet_pton(AF_INET, server_ip.c_str(), &addr.sin_addr) <= 0) {
        std::cerr << "Endereço IP inválido\n";
        return false;
    }

    ssize_t sent = sendto(sockfd, data.c_str(), data.size(), 0,
                          (sockaddr *)&addr, sizeof(addr));

    return sent == (ssize_t)data.size();
}
