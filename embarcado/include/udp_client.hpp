/**
 * @file udp_client.hpp
 * @brief Declaração da classe UDPClient responsável por envio de dados via protocolo UDP.
 *
 * Esta classe fornece uma interface simples para envio de mensagens
 * pela rede utilizando o protocolo UDP (User Datagram Protocol).
 * É ideal para aplicações leves de comunicação, como transmissão
 * de dados de sensores para um servidor.
 */

#ifndef UDP_CLIENT_HPP
#define UDP_CLIENT_HPP

#include <string>

/**
 * @class UDPClient
 * @brief Classe para envio de dados via UDP.
 *
 * A classe abstrai a criação do socket, configuração do endereço
 * do servidor e envio de pacotes de dados no formato texto.
 */
class UDPClient {
public:
    /**
     * @brief Construtor da classe UDPClient.
     *
     * Cria um cliente UDP e armazena o endereço IP e a porta do servidor
     * para o qual os dados serão enviados.
     *
     * @param ip Endereço IP do servidor de destino (ex: "192.168.0.10").
     * @param port Porta UDP do servidor de destino.
     */
    UDPClient(const std::string &ip, int port);

    /**
     * @brief Destrutor da classe UDPClient.
     *
     * Fecha o socket UDP, se estiver aberto.
     */
    ~UDPClient();

    /**
     * @brief Envia dados via UDP para o servidor configurado.
     *
     * @param data String contendo os dados a serem enviados.
     * @return `true` se o envio for bem-sucedido, `false` em caso de erro.
     */
    bool sendData(const std::string &data);

private:
    int sockfd;              /**< Descritor do socket UDP. */
    std::string server_ip;   /**< Endereço IP do servidor de destino. */
    int server_port;         /**< Porta UDP do servidor de destino. */
};

#endif // UDP_CLIENT_HPP
