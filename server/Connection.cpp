/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** Connection
*/

/// \file Connection.cpp
/// \author Mattéo V.
/// \brief Source file for Connection class

#include "Connection.hpp"

/// \brief constructor.
/// Initialize socket
/// \param io : boost's io_context used by every I/O object in boost::asio
Serv::Connection::Connection(boost::asio::io_context &io)
    : _socket(io)
{}

/// \brief Destructor.
/// Do nothing special
Serv::Connection::~Connection() {}

/// \param io : boost's io_contect used by every I/O object in boost::asio
/// \return new Connection instance as a shared pointer
/// \brief Return a new Connection instance as a shared pointer
boost::shared_ptr<Serv::Connection> Serv::Connection::create(boost::asio::io_context &io)
{
    return boost::shared_ptr<Serv::Connection>(new Serv::Connection(io));
}

/// \return Socket reference from the class
/// \brief Return the socket
tcp::socket &Serv::Connection::getSocket()
{
    return _socket;
}

/// \param db : smart pointer to the database
/// \brief Set the database pointer, this function is called on Server class and _db is used for database requests
void Serv::Connection::setDb(std::shared_ptr<Serv::DataBase> &db)
{
    _db = db;
}

/// \param ip : ip address of the current client connected to this instance of Connection
/// \brief Save the ip of the remote client in the instance of Connection, called in Server when the connection occurs
void Serv::Connection::setIP(const std::string &ip)
{
    _ip = ip;
}

/// \brief server's startup function. Called when a client is connected to bind writing and
/// reading functions to their respectives callback
void Serv::Connection::start()
{
    read();
}

/// \param error : error code set by boost::asio
/// \param b : number of bytes written
/// \brief Callback function used after a packet has been sent, if an error occurs, disconnect the client from the server
void Serv::Connection::handleWrite(const boost::system::error_code &error, UNUSED size_t b)
{
    if (!error)
        std::cout << "Packet sent to " << _ip << std::endl;
    else
        disconnect(error);
}

/// \param error : error code set by boost::asio
/// \param b : number of bytes written
/// \brief Callback function used after a packet has been received
/// analyse the received packet to return what the client asked (if INFO_CONTACT is send, return client's contacts)
void Serv::Connection::handleRead(const boost::system::error_code &error, size_t b)
{
    Babel::tcpHeader *received = static_cast<Babel::tcpHeader *>((void *)_buf.data());
    std::cout << b << " bytes packet received from: " + _ip << std::endl;
    std::cout << "\tCODE='" << received->code << "'" << std::endl;
    std::cout << "\tMSG='" << received->data << "'" << std::endl;

    if (received->code == INFO_AUTH) {
        std::vector<std::string> res;
        boost::split(res, received->data, [](char c){ return c == '-'; });
        _name = res[0];

        if (userExist(res[0]))
            login(res[0], res[1]);
        else
            addUser(res[0], res[1]);
    } else if (received->code == INFO_CONTACT) {
        std::string name = received->data;
        _name = name;
        searchContacts(name);
    } else if (received->code == ADD_CONTACT) {
        std::string name = received->data;
        addContact(name);
    }
    for (int i = 0; i < BUFFER_SIZE; i++)
        _buf[i] = 0;
    if (error)
        disconnect(error);
    else
        read();
}

/// \param error : error code set by boost::asio, used to display the error message
/// \brief Show the error message and disconnect the socket
void Serv::Connection::disconnect(const boost::system::error_code &error)
{
    std::cout << "Disconnect client with ip: " + _ip << std::endl;

    _db->update("user", "ip_addr = '0.0.0.0'", "username=\"" + _name + "\"");
    if (error.value() != 2)
        std::cerr << "Error: " << error.message() << std::endl;
    _socket.close();
}

/// \brief Bind the read action with a callback, must be called by the callback to read again on network
void Serv::Connection::read()
{
    _socket.async_read_some(boost::asio::buffer(_buf),
        boost::bind(&Connection::handleRead, shared_from_this(),
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

/// \param code : network code to be written on the network (see Voip.hpp for all codes available)
/// \param data : string to be written on network with network code
/// \param size : size of the string to be written on network
/// \brief Write a packet on network, use the structure defined in Voip.hpp to send data
void Serv::Connection::writeback(uint code, const char *data, std::size_t size)
{
    Babel::tcpHeader *toSend = new Babel::tcpHeader();
    toSend->code = code;
    std::memset(&(toSend->data), 0, TCP_BUFFER_SIZE);
    std::strcpy((char *) &(toSend->data), data);
    std::cout << "Packet will be send to: " + _ip << " with code: " << toSend->code << " and message: '" << toSend->data << "'" << std::endl;
    _socket.async_write_some(boost::asio::buffer(static_cast<void *>(toSend), 4 + size),
        boost::bind(&Connection::handleWrite, shared_from_this(),
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
    delete toSend;
}

/// \param name : username of user
/// \brief Check in the database if a specified user exist to know if we need to register or login
/// \return true if the user is already registered otherwise false
bool Serv::Connection::userExist(const std::string &name)
{
    std::vector<std::unordered_map<std::string, std::string>> res = _db->select("username", "user", "");
    for (long unsigned int i = 0; i < res.size(); i++)
        if (res[i]["username"] == name)
            return true;
    return false;
}

/// \param name : username of the user to register
/// \param passwd : password of the user to register
/// \brief Register a new user in the DataBase and write on the network that the user is connected
void Serv::Connection::addUser(const std::string &name, const std::string &passwd)
{
    _db->insert("user (username, password, contacts, ip_addr)", {"('" + name + "'", "'" + passwd + "'", "''", "'" + _ip + "')"});
    writeback(INFO_AUTH_OK, "", 1);
}

/// \param name : username of the user to login
/// \param passwd : password of the user to login
/// \brief Log in the user if password is correct and send INFO_AUTH_OK to client, otherwise send INFO_AUTH_KO
void Serv::Connection::login(const std::string &name, const std::string &passwd)
{
    std::vector<std::unordered_map<std::string, std::string>> res = _db->select("password", "user", "username=\"" + name + "\"");

    if (res[0]["password"] == passwd) {
        _db->update("user", "ip_addr = '" + _ip + "'", "username=\"" + name + "\"");
        writeback(INFO_AUTH_OK, "", 1);
    } else
        writeback(INFO_AUTH_KO, "", 1);
}

/// \param name : username of the user
/// \brief Search in DataBase the contacts of the specified user and send it back to client when found
void Serv::Connection::searchContacts(const std::string &name)
{
    std::vector<std::unordered_map<std::string, std::string>> res = _db->select("contacts", "user", "username=\"" + name + "\"");
    std::vector<std::string> result;

    if (res.size() == 0) {
        writeback(ERROR_SERVER, "\0", 2);
        return;
    } else if (res[0].count("contacts") == 0) {
        writeback(INFO_CONTACT, "\0", 2);
        return;
    } else
        boost::split(result, res[0]["contacts"], [](char c){ return c == ';'; });

    std::string where = "";
    for (long unsigned int i = 0; i < result.size(); i++)
        where += "id = " + result[i] + (i != result.size() - 1 ? " OR " : "");
    std::vector<std::unordered_map<std::string, std::string>> contacts = _db->select("username, ip_addr", "user", where);
    std::string toSend = "";
    for (long unsigned int i = 0; i < contacts.size(); i++)
        toSend += contacts[i]["username"] + "-" + contacts[0]["ip_addr"] + (i != contacts.size() - 1 ? ";" : "");
    writeback(INFO_CONTACT, toSend.c_str(), toSend.size() + 1);
}

/// \param name : username of the user
/// \brief Add a contact for 2 users
void Serv::Connection::addContact(const std::string &name)
{
    // id = 3
    std::vector<std::unordered_map<std::string, std::string>> contacts = _db->select("id, contacts", "user", "username=\"" + _name + "\"");
    std::vector<std::string> contactListClient1;
    if (contacts.size() == 0) {
        writeback(ERROR_SERVER, "\0", 2);
        return;
    } else if (contacts[0].count("contacts") == 0)
        contactListClient1 = { "" };
    else
        boost::split(contactListClient1, contacts[0]["contacts"], [](char c){ return c == ';'; });
    // contactListClient1 = {"1"}
    std::string id1 = contacts[0]["id"];
    if (contactListClient1.size() >= 5) {
        writeback(ERROR_SERVER, "\0", 2);
        return;
    }

    // id = 2
    std::vector<std::unordered_map<std::string, std::string>> newContact = _db->select("id, contacts", "user", "username=\"" + name + "\"");
    std::string contactListClient2;
    if (newContact.size() == 0) {
        writeback(ERROR_SERVER, "\0", 2);
        return;
    } else if (newContact[0].count("contacts") == 0)
        contactListClient2 = "";
    else
        contactListClient2 = newContact[0]["contacts"];
    // contactListClient2 = "1"
    std::string id2 = newContact[0]["id"];

    contactListClient1.push_back(id2);
    if (isInList(contactListClient2, id1))
        return;
    contactListClient2 += (contactListClient2.size() > 0 ? ";" : "") + id1;
    std::string newContactListClient1 = "";
    for (long unsigned int i = 0; i < contactListClient1.size(); i++)
        newContactListClient1 += contactListClient1[i] + (i != contactListClient1.size() - 1 ? ";" : "");
    // newContactListClient1 = "1;2"
    // contactListClient2 = "1;3"
    std::cout << newContactListClient1 << " " << contactListClient2 << std::endl;
    _db->update("user", "contacts = '" + newContactListClient1 + "'", "username=\"" + _name + "\"");
    _db->update("user", "contacts = '" + contactListClient2 + "'", "username=\"" + name + "\"");
}

/// \return true if the specified element is in the list
/// \param list : list of contact
/// \param test : contact to find
/// \brief Test if a specified contact is already in the contact list
bool Serv::Connection::isInList(const std::string &list, const std::string &test)
{
    std::vector<std::string> l;
    boost::split(l, list, [](char c){ return c == ';'; });

    for (long unsigned int i = 0; i < l.size(); i++)
        if (l[i] == test)
            return true;
    return false;
}
