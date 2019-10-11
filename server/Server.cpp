/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** Server
*/

/// \file Server.cpp
/// \author Matteo V.
/// \brief Server main class

#include "Server.hpp"

/// \brief Constructor
/// Initialize network acceptor and call startAccept
Serv::Server::Server(boost::asio::io_context &io)
    : _acceptor(io, tcp::endpoint(tcp::v4(), SIP_PORT)), _db(std::make_shared<Serv::DataBase>("babel.db"))
{
    initDb();
    startAccept();
}

/// \brief Destructor
/// Do nothing special
Serv::Server::~Server() {}

/// Set a callback function to handle new connections, is called again by handleAccept
/// and wait for an other connection to come
void Serv::Server::startAccept()
{
    boost::shared_ptr<Serv::Connection> co = Serv::Connection::create(_acceptor.get_executor().context());
    co->setDb(_db);
    _acceptor.async_accept(co->getSocket(),
        boost::bind(&Server::handleAccept, this, co, boost::asio::placeholders::error));
}

/// \param co : instance of Connection Class wrapped in a smart pointer
/// \param error : error code given by boost if something went wrong
/// Handle new connection to the server
void Serv::Server::handleAccept(
    boost::shared_ptr<Serv::Connection> &co,
    const boost::system::error_code &error
)
{
    if (!error) {
        const std::string ip = co->getSocket().remote_endpoint().address().to_string();
        const ushort port = co->getSocket().remote_endpoint().port();
        std::cout << "Client: " << ip << " " << port << std::endl;
        co->setIP(ip);
        co->start();
    }
    // call startAccept to handle an other connection
    startAccept();
}

/// Initialize database table, called in the class ctor
void Serv::Server::initDb()
{
    std::vector<std::pair<std::string, std::string>> col = {
        {"id", "INTEGER PRIMARY KEY AUTOINCREMENT, "},
        {"username", "VARCHAR(" + std::to_string(SIZE_NAME) + "), "},
        {"password", "VARCHAR(" + std::to_string(PASS_SIZE) + "), "},
        {"contacts", "VARCHAR(" + std::to_string(SIZE_NAME * CONTACT_MAX) + "), "},
        {"ip_addr", "VARCHAR(" + std::to_string(IP_SIZE) + ")"}
    };

    try {
        _db->createTable("user", col);
    } catch (Babel::Exception &e) {
        throw e;
    }
}
