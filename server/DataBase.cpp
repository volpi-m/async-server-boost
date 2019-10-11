/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** Database Object
*/

/// \file DataBase.cpp
/// \author Matteo V. Arthur L.
/// \brief DataBase implementation file

#include "DataBase.hpp"

/// \brief Constructor.
/// Open (or create) the database.
/// Throw a Babel::Exception if something went wrong with sqlite3
/// \param dbName : Name of the database to be opened (or created)
Serv::DataBase::DataBase(const std::string &dbName) : _db(nullptr), _errorMsg(nullptr)
{
    int quit = sqlite3_open(dbName.c_str(), &_db);

    if (quit)
        throw Babel::Exception("Can't open " + dbName + " Data base", "DataBase constructor");    
}

/// \brief Destructor.
/// Free _errorMsg and close the connection to database
Serv::DataBase::~DataBase()
{
    if (_errorMsg != nullptr)
        sqlite3_free(_errorMsg);
    sqlite3_close(_db);
}

/// \param table : name of the table you want to insert something into
/// \param values : all values needed in the table
/// \brief Insert values in the specified table.
/// If insertion is not possible throw a Babel::Exception
void Serv::DataBase::insert(const std::string &table, const std::vector<std::string> &values)
{
    std::string querry = "INSERT INTO " + table + " VALUES ";
    unsigned int count = 0;
    for (auto i : values) {
        querry += i;
        if (++count < values.size())
            querry += ", ";
    }
    querry += ";";
    std::cout << "Querry insert: " << querry << std::endl;
    if (sqlite3_exec(_db, querry.c_str(), 0, nullptr, &_errorMsg) != SQLITE_OK)
        std::cerr << _errorMsg << std::endl;
}

/// \param name : name of the table to delete
/// \brief Drop the 'name' table from the current database.
/// Drop a table is a dangerous action, be careful when using it.
/// Throw a Babel::Exception if connection with database is impossible
void Serv::DataBase::deleteTable(const std::string &name)
{
    std::string querry = "DROP TABLE " + name;
    if (sqlite3_exec(_db, querry.c_str(), 0, nullptr, &_errorMsg) != SQLITE_OK)
        throw Babel::Exception(_errorMsg, "DataBase deleteTable");
}

/// \param name : name of the table to be created
/// \param columns : vector containing a pair of strings, first string is the columns' name and second string is the columns' type
/// \brief Create a table named 'name' and create all requested columns with their respective types
void Serv::DataBase::createTable(
    const std::string &name,
    std::vector<std::pair<std::string, std::string>> &columns
)
{
    std::string querry = "CREATE TABLE IF NOT EXISTS " + name + "(";
    for (auto i : columns)
        querry += i.first + " " + i.second;
    querry += ");";
    std::cout << "Querry create table: " << querry << std::endl;
    if (sqlite3_exec(_db, querry.c_str(), 0, nullptr, &_errorMsg) != SQLITE_OK)
        throw Babel::Exception(_errorMsg, "DataBase createTable");
}

/// \param table : name of the table
/// \param value : new value
/// \param where : criteria
/// \brief Update the value 'value' in the table 'table'. You can use 'where' for changing multiple values.
void Serv::DataBase::update(
    const std::string &table,
    const std::string &value,
    const std::string &where
)
{
    std::string querry = "UPDATE " + table + " SET " + value;
    if (where != "")
        querry += " WHERE " + where;
    querry += ";";
    std::cout << "Querry update: " << querry << std::endl;
    if (sqlite3_exec(_db, querry.c_str(), 0, nullptr, &_errorMsg) != SQLITE_OK)
        std::cerr << "Error: Failed to update " + value + " in DataBase" << std::endl;
}

/// \param res : containers fill with all selected data
/// \param argc : size of data array
/// \param argv : array of selected data
/// \param colName : name of column in data base
/// \brief Call back function call to transform 'argv' array in 'res' container
/// \return return 0
int Serv::DataBase::selectCallback(void *res, int argc, char **argv, char **colName)
{
    std::unordered_map<std::string, std::string> tmp;
    if (res == nullptr)
        throw Babel::Exception("Data container pointer is null", "DataBase select");
    for (int i = 0; i < argc; i++)
        tmp[colName[i]] = argv[i] ? argv[i] : NULL;
    static_cast<std::vector<std::unordered_map<std::string, std::string>> *>(res)->push_back(tmp);
    return (0);
}

/// \param row : name of the values you want to select
/// \param table : table where datas are located
/// \param where : special selection in the table
/// \brief It's an implementation of the SELECT sql function. It return a certain set of datas contained in the data base.
/// \return Return a vector of map. Each element of the vector is a line of the DataBase,
/// inside each unordered_map a value is mapped with its column name 
std::vector<std::unordered_map<std::string, std::string>> Serv::DataBase::select
    (const std::string &row, const std::string &table, const std::string &where)
{
    std::vector<std::unordered_map<std::string, std::string>> res;
    std::string querry = "SELECT " + row + " FROM " + table;
    if (where != "")
        querry += " WHERE " + where;
    querry += ";";
    std::cout << "Querry select: " << querry << std::endl;
    if (sqlite3_exec(_db, querry.c_str(), selectCallback, &res, &_errorMsg) != SQLITE_OK)
        std::cerr << "Error: Failed to select " + row + " from " + table << std::endl;
    return (res);
}

/// \param elem : element(s) to delete
/// \param table : table where you want delete some datas
/// \param where : special criteria for deletion
/// \brief Implementation of DELETE function in SQL. It's allow to remove some data 'elem' from 'table'.
void Serv::DataBase::del(const std::string &elem, const std::string &table, const std::string &where)
{
    std::string querry = "DELETE " + elem + " FROM " + table;
    if (where != "")
        querry += (" WHERE " + where);
    querry += ";";
    if (sqlite3_exec(_db, querry.c_str(), 0, nullptr, &_errorMsg) != SQLITE_OK)
        std::cerr << "Error: Failed to delete " + elem + " from " + table << std::endl;
}
