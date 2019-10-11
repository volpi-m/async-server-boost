/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** Server
*/

/// \file Server.hpp
/// \author Mattéo V.
/// \brief Header file for Server class

#ifndef SERVER_HPP_
    #define SERVER_HPP_

#include <iostream>
#include <memory>
#include <list>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include "Connection.hpp"
#include "DataBase.hpp"

/// \def SIZE_NAME
/// \brief Maximum size of a username that can be stored in the database
#define SIZE_NAME 16

/// \def PASS_SIZE
/// \brief Password maximum size
#define PASS_SIZE 64

/// \def CONTACT_MAX
/// \brief Maximum number of contact that can be stored in the DataBase for a single user
#define CONTACT_MAX 5

/// \def IP_SIZE
/// \brief Size of an IP address (string)
#define IP_SIZE 16

/// \namespace Serv
/// \brief Used for the all server classes
namespace Serv
{
    /// \class Server
    /// \brief Server class, used in parallel with Connection class
    class Server
    {
    public:
        Server(boost::asio::io_context &);
        ~Server();

    private:
        void startAccept();
        void handleAccept(boost::shared_ptr<Serv::Connection> &, const boost::system::error_code &);
        void initDb();

        /*! Acceptor use by boost to accept input connections */
        tcp::acceptor _acceptor;
        /*! DataBase shared pointer given in Server constructor by ServerManager */
        std::shared_ptr<Serv::DataBase> _db;
    };
}

#endif /* !SERVER_HPP_ */
