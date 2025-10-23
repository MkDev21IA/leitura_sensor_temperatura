/**
 * @file data_formatter.cpp
 * @brief Implementação das funções de formatação de pacotes de dados UDP.
 *
 * Este módulo define a função responsável por preparar pacotes
 * de dados de sensores no formato `UdpPacket`, incluindo a geração
 * automática de timestamp e a associação de metadados como grupo,
 * sensor, valor e unidade.
 */

#include "data_formatter.hpp"

/**
 * @namespace DataFormatter
 * @brief Funções auxiliares para formatação de dados de sensores.
 *
 * Este namespace agrupa utilitários responsáveis por montar pacotes
 * de dados antes do envio via protocolo UDP.
 */
namespace DataFormatter {

    /**
     * @brief Prepara um pacote de dados no formato `UdpPacket`.
     *
     * Constrói uma estrutura `UdpPacket` preenchendo os campos
     * com as informações do sensor e adiciona automaticamente
     * um timestamp no formato ISO8601 (UTC).
     *
     * Exemplo de uso:
     * @code
     * UdpPacket pkt = DataFormatter::preparePacket("Temperatura", "Sensor1", 25.4, "°C");
     * std::string json = serialize(pkt);
     * @endcode
     *
     * @param group Nome do grupo ou categoria do sensor (ex: "Temperatura").
     * @param sensor Identificador do sensor (ex: "Sensor1").
     * @param value Valor medido pelo sensor.
     * @param unit Unidade de medida (ex: "°C", "V").
     * @return Estrutura `UdpPacket` pronta para serialização e envio.
     */
    UdpPacket preparePacket(const std::string &group,
                            const std::string &sensor,
                            double value,
                            const std::string &unit)
    {
        UdpPacket p;
        p.group_id = group;
        p.sensor_id = sensor;
        p.value = value;
        p.unit = unit;
        p.timestamp = currentTimestamp();
        return p;
    }
}
