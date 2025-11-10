/**
 * @file data_formatter.hpp
 * @brief Declarações para formatação e preparação de pacotes de dados UDP.
 *
 * Este arquivo define a função responsável por formatar dados de sensores
 * em um pacote UDP no formato esperado pelo protocolo definido em
 * `udp_protocol.hpp`.
 */

#ifndef DATA_FORMATTER_HPP
#define DATA_FORMATTER_HPP

#include <string>
#include "udp_protocol.hpp"

/**
 * @namespace DataFormatter
 * @brief Namespace para funções de formatação de dados de sensores.
 *
 * Contém utilitários para preparar pacotes de dados antes do envio pela rede.
 */
namespace DataFormatter {

    /**
     * @brief Prepara um pacote UDP com informações do sensor.
     *
     * Esta função formata os dados recebidos (grupo, sensor, valor e unidade)
     * em uma estrutura `UdpPacket` pronta para envio via UDP.
     *
     * @param group Nome do grupo ou categoria do sensor (ex: "Temperatura").
     * @param sensor Identificador do sensor (ex: "Sensor1").
     * @param value Valor medido pelo sensor.
     * @param unit Unidade do valor (ex: "°C", "V", "m/s²").
     * @return Estrutura `UdpPacket` contendo os dados formatados.
     */
    UdpPacket preparePacket(const std::string &group,
                            const std::string &sensor,
                            double value,
                            const std::string &unit);
}

#endif // DATA_FORMATTER_HPP

