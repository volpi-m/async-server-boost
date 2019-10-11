/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** Exception handling
*/

/// \file Exception.hpp
/// \author Arthur L. Matteo V.
/// \brief Header file for Exception class

#pragma once

#include <exception>
#include <string>
#include <iostream>

/// \def EXIT_SUCCESS
/// \brief Value returned by the program if everything worked fine
#define EXIT_SUCCESS 0

/// \def EXIT_FAIL
/// \brief Value returned by the program if something went wrong
#define EXIT_FAIL 84

/// \namespace Babel
/// \brief Babel namespace, used in both client and server
namespace Babel
{
    /// \class Exception, inherited from standard exception class
    /// \brief Exception class used on top of standard exception class
    class Exception : public std::exception
    {
        public:
            Exception() noexcept = delete;
            ~Exception() override = default;
            explicit Exception(std::string what, std::string where = "") noexcept;
            explicit Exception(const char *what, const char *where = "") noexcept;
            Exception(const Exception &other) = default;
            Exception &operator=(const Exception &other) = default;

            static void displayException(const Babel::Exception &e);

            char const *what() const noexcept override;
            virtual char const *where() const noexcept;

        protected:
            /*! String set to an error message when the exception is thrown */
            std::string _what;
            /*! String set to the function name where the exception is thrown */
            std::string _where;
    };
}