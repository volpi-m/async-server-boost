/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** DataBase interface
*/

/// \file IDataBase.hpp
/// \author Arthur L. Mattéo V.
/// \brief Header file for DataBase interface: IDataBase
/// As long as you implement all the functions in this class you can use your own database class

#pragma once

/// \namespace Serv
/// \brief Used for the all server classes
namespace Serv
{
    /// \class IDataBase
    /// \brief Interface for the DataBase class so the program can use an other database class
    /// as long as the class is inheriting the IDataBase interface
    class IDataBase
    {
    public:
        virtual ~IDataBase() = default;
        virtual void createTable(const std::string &name, std::vector<std::pair<std::string, std::string>> &columns) = 0;
        virtual void deleteTable(const std::string &name) = 0;
        virtual void insert(const std::string &table, const std::vector<std::string> &values) = 0;
        virtual void update(const std::string &table, const std::string &value, const std::string &where = "") = 0;

        virtual std::vector<std::unordered_map<std::string, std::string>> select (const std::string &row, const std::string &table, const std::string &where = "") = 0;
        virtual void del(const std::string &elem, const std::string &table, const std::string &where = "") = 0;
    };
}
