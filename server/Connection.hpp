/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** Connection
*/

/// \file Connection.hpp
/// \author Mattéo V.
/// \brief Header file for Connection class

#ifndef CONNECTION_HPP_
    #define CONNECTION_HPP_

#include <iostream>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>

#include "Voip.hpp"
#include "DataBase.hpp"

/// \def UNUSED
/// \brief attribute unused from g++
#define UNUSED __attribute__((unused))

using boost::asio::ip::tcp;

/// \def BUFFER_SIZE
/// \brief Size of the buffer filled with info that has been read on network
#define BUFFER_SIZE 256

/// \namespace Serv
/// \brief Used for the all server classes
namespace Serv
{
    class Server;
    /// \class Connection
    /// \brief Connection class, asynchronous server, handle all I/O connections with clients
    class Connection : public boost::enable_shared_from_this<Connection>
    {
    public:
        ~Connection();

        static boost::shared_ptr<Connection> create(boost::asio::io_context &);
        tcp::socket &getSocket();
        void setDb(std::shared_ptr<Serv::DataBase> &);
        void setIP(const std::string &ip);
        void start();

    private:
        Connection(boost::asio::io_context &);
        void handleWrite(const boost::system::error_code &, size_t);
        void handleRead(const boost::system::error_code &, size_t);
        void disconnect(const boost::system::error_code &);
        void read();
        void writeback(uint, const char *, std::size_t);

        bool userExist(const std::string &);
        void addUser(const std::string &, const std::string &);
        void login(const std::string &, const std::string &);
        void searchContacts(const std::string &);
        void addContact(const std::string &);
        bool isInList(const std::string &, const std::string &);

        /*! Socket used for the connection with a client */
        tcp::socket _socket;
        /*! Buffer filled by received packets */
        boost::array<char, BUFFER_SIZE> _buf;
        /*! DataBase smart pointer given by Server */
        std::shared_ptr<Serv::DataBase> _db;
        /*! IP address of the client connected to this instance of the Connection class */
        std::string _ip;
        /*! Username of the client connected to this instance of the Connection class */
        std::string _name;
    };
}

#endif /* !CONNECTION_HPP_ */
