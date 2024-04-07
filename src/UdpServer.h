#include <WiFiUdp.h>
#include "Logger.h"
#include <WakeOnLan.h>
#include <iostream>
#include <string>
#include <algorithm>

#define UDP_PORT 9 // WoL port
#define WOL_REPEAT 3
#define WOL_INTERVAL_MS 100

class UdpServer
{
private:
    // UDP variables
    WiFiUDP _udpService;
    Logger _logger;

    char packet[255];

public:
    UdpServer(Logger &logger)
    {

        // Begin listening to UDP port
        _logger = logger;
    };

    /// @brief Start the service
    void Start()
    {
        if (_udpService.begin(UDP_PORT))
            _logger.Log("UDP service started!");
    }

    /// @brief Try reading a packet from the service
    bool ParsePacket()
    {
        auto res = _udpService.parsePacket();

        if (!(res > 0))
            return false;
        _logger.Log("Packet received!");

        int len = _udpService.read(packet, 255);

        if (!(len > 0))
            return false;

        _logger.Log("Packet parsed!");
        packet[len] = '\0';

        return true;
    };

    /// @brief  Gets last rad packet from the UDP service
    /// @return
    char *GetLastPacket()
    {
        return packet;
    }

    /// @brief Send a WoL magic packet to the desired IP address and port for 3 times and every 100ms
    /// @param remoteIp
    /// @param remoteMacAddress Specify the macAddress of the target machine
    /// @param remotePort
    void SendWakeOnLan(IPAddress broadcastAddress, String remoteMacAddress, int remotePort = 9)
    {

        WakeOnLan wakeOnLan = WakeOnLan(_udpService);

        _logger.LogInline("Sending Wake on Lan magic packet on the broadcast address ");
        _logger.Log(broadcastAddress.toString());

        wakeOnLan.setBroadcastAddress(broadcastAddress);
        wakeOnLan.setRepeat(WOL_REPEAT, WOL_INTERVAL_MS);
        wakeOnLan.sendMagicPacket(remoteMacAddress);
    }

    // Function to extract and format the MAC address from a magic packet
    std::string ExtractMacAddressFromMagicPacket(const std::string &packet)
    {
        // The MAC address starts at the 6th byte and is 6 bytes long in a WoL packet
        if (packet.length() < 12)
        {
            return "";
        }

        std::string macAddress;
        for (size_t i = 6; i < 12; ++i)
        {
            char buffer[3];
            sprintf(buffer, "%02x", static_cast<unsigned char>(packet[i]));
            macAddress += buffer;
            if (i != 11)
            {
                macAddress += ":";
            }
        }

        return macAddress;
    }
};