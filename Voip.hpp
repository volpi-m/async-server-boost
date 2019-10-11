/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** Macro for VOIP protocole
*/

/// \file Voip.hpp
/// \author Matteo V. Arthur L.
/// \brief Header file used by both client and server to get information about network communication.
/// Contain communication codes and the structure used to send data.

#pragma once

#include "PortAudioWrapper.hpp"

/// \def INFO_AUDIO
/// \brief Code used by client when he send audio.
#define INFO_AUDIO 101

/// \def INFO_AUTH
/// \brief Code used to authentified an user
#define INFO_AUTH 111

/// \def INFO_AUTH_OK
/// \brief Code send to client when a successful login happened
#define INFO_AUTH_OK 112

/// \def INFO_AUTH_KO
/// \brief Code send to client when a bad login happened
#define INFO_AUTH_KO 113

/// \def ADD_CONTACT
/// \brief Code used to add a contact, client send the code + the username of the contact
#define ADD_CONTACT 137

/// \def INFO_CONTACT
/// \brief Code used to request contact list from server
#define INFO_CONTACT 142

/// \def INFO_CALL
/// \brief Code used by client to notified another client that he's calling him.
#define INFO_CALL 180

/// \def INFO_HANG_UP
/// \brief Code when user hang up
#define INFO_HANG_UP 183

/// \def SUCCESS_ACCEPT
/// \brief Code used by client to notified a calling client that he accept his call
#define SUCCESS_ACCEPT 202 // accept call

/// \def ERROR_CLIENT_NOT_FOUND
/// \brief Error code when client is not found
#define ERROR_CLIENT_NOT_FOUND 404

/// \def ERROR_CLIENT_TIMEOUT
/// \brief Error code when client timeout
#define ERROR_CLIENT_TIMEOUT 408

/// \def ERROR_SERVER
/// \brief Error code when server encounter an error
#define ERROR_SERVER 500

/// \def IP_SERVER
/// \brief Ip address for the server
#define IP_SERVER "10.18.207.185"

/// \def SIP_PORT
/// \brief Port used by server and client sor Session Initialization Protocol (SIP)
#define SIP_PORT 5060

/// \def UDP_PORT
/// \brief Port used by clients to communicate beetween them
#define UDP_PORT 5065

/// \def TCP_BUFFER_SIZE
/// \brief Size of buffer send on network by client and server to communicate
#define TCP_BUFFER_SIZE 256

/// \namespace
/// \brief Babel namespace, used in both client and server
namespace Babel
{
    // Packed struct, send on network, so smaller is better
    #pragma pack(push, 1)
    /// \struct tcpHeader Voip.hpp
    /// \param code : Communication code used by the client to request specific information and
    /// used by the server to return it
    /// \param data : Buffer used to write data to send if needed
    /// \brief Structure used to send data between client and server
    struct tcpHeader
    {
        unsigned int code;
        char data[TCP_BUFFER_SIZE];
    };
    // End of packing
    #pragma pack(pop)

    /// \Class udpHeader
    /// Header for all udp communication.
   	struct udpHeader
	{
        unsigned int code;
		int id;
		char *compressionLib;
		char data[FRAMES_PER_BUFFER];
	};
}
