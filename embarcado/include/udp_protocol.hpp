/**
 * @file udp_protocol.hpp
 * @brief Estruturas e funções auxiliares para o envio e formatação de pacotes UDP em formato JSON.
 *
 * Este arquivo define a estrutura de dados `UdpPacket` e funções utilitárias
 * para gerar timestamps e serializar os pacotes no formato JSON,
 * utilizados na comunicação entre sensores e o servidor.
 */

#ifndef UDP_PROTOCOL_HPP
#define UDP_PROTOCOL_HPP

#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>

/**
 * @struct UdpPacket
 * @brief Representa um pacote de dados UDP no formato JSON.
 *
 * Contém as informações básicas enviadas por um sensor, incluindo
 * o identificador do grupo, o ID do sensor, o valor medido, a unidade
 * e o timestamp UTC da leitura.
 */
struct UdpPacket {
    std::string group_id;   /**< Identificador do grupo de sensores (ex: "Temperatura"). */
    std::string sensor_id;  /**< Identificador único do sensor (ex: "Sensor1"). */
    double value;           /**< Valor medido pelo sensor. */
    std::string unit;       /**< Unidade de medida (ex: "°C", "V"). */
    std::string timestamp;  /**< Data/hora da leitura no formato ISO8601 (UTC). */
};

/**
 * @brief Gera um timestamp atual em UTC no formato ISO8601.
 *
 * Exemplo de saída: `"2025-10-22T22:15:30Z"`.
 *
 * @return String contendo o timestamp atual no formato ISO8601.
 */
inline std::string currentTimestamp() {
    std::time_t t = std::time(nullptr);
    std::tm *gmt = std::gmtime(&t);
    char buf[64];
    std::strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", gmt);
    return std::string(buf);
}

/**
 * @brief Serializa uma estrutura `UdpPacket` em formato JSON.
 *
 * Converte os dados do pacote para uma string JSON simples,
 * adequada para envio via rede UDP.
 *
 * Exemplo de saída:
 * @code
 * {
 *   "group":"Temperatura",
 *   "sensor_id":"Sensor1",
 *   "value":25.34,
 *   "unit":"°C",
 *   "ts":"2025-10-22T22:15:30Z"
 * }
 * @endcode
 *
 * @param p Estrutura `UdpPacket` a ser serializada.
 * @return String contendo os dados do pacote em formato JSON.
 */
inline std::string serialize(const UdpPacket &p) {
    std::ostringstream oss;
    oss << "{";
    oss << "\"group\":\"" << p.group_id << "\",";
    oss << "\"sensor_id\":\"" << p.sensor_id << "\",";
    oss << "\"value\":" << std::fixed << std::setprecision(2) << p.value << ",";
    oss << "\"unit\":\"" << p.unit << "\",";
    oss << "\"ts\":\"" << p.timestamp << "\"";
    oss << "}";
    return oss.str();
}

#endif // UDP_PROTOCOL_HPP

