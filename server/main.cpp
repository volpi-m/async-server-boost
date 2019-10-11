/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** main
*/

#include "Server.hpp"

int main()
{
    try {
        boost::asio::io_context io;
        Serv::Server serv(io);
        io.run();
    } catch(const Babel::Exception &e) {
        Babel::Exception::displayException(e);
    }
    return 0;
}
