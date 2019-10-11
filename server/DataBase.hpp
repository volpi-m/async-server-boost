/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** Database class
*/

/// \file DataBase.hpp
/// \author Arthur L. Matteo V.
/// \brief Header file for DataBase class

#pragma once

#include <iostream>
#include <string>
#include <sqlite3.h>
#include <vector>
#include <unordered_map>
#include <map>
#include <utility>

#include "IDataBase.hpp"
#include "Exception.hpp"

/// \namespace Serv
/// \brief Used for the all server classes
namespace Serv
{
    /// \class DataBase
    /// \brief DataBase class used to communicate with a sqlite DataBase, inherited from IDataBase interface
    class DataBase : public Serv::IDataBase
    {
    public:
        explicit DataBase(const std::string &dbName);
        ~DataBase() override;

        void createTable(const std::string &name, std::vector<std::pair<std::string, std::string>> &columns);
        void deleteTable(const std::string &name);
        void insert(const std::string &table, const std::vector<std::string> &values); // no specific insertion possible
        void update(const std::string &table, const std::string &value, const std::string &where = "");

        std::vector<std::unordered_map<std::string, std::string>> select
            (const std::string &row, const std::string &table, const std::string &where = "");
        void del(const std::string &elem, const std::string &table, const std::string &where = "");

    private:
        /*! sqlite connection point */
        sqlite3 *_db;
        /*! Error message set by sqlite3 library and then thrown as an exception message */
        char *_errorMsg;

        static int selectCallback(void *data, int argc, char **argv, char **azColName);
    };
}
