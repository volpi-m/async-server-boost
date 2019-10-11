/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** Exception Handling
*/

#include "Exception.hpp"

/// \brief Constructor.
/// Initialize _what and _where
Babel::Exception::Exception(const char *what, const char *where) noexcept
	: _what(what), _where(where)
{}

/// \brief Constructor.
/// Initialize _what and _where by using std::move on strings given
Babel::Exception::Exception(std::string what, std::string where) noexcept
	: _what(std::move(what)), _where(std::move(where))
{}

/// \return Return _what
/// \brief Return the description of the exception
char const *Babel::Exception::what() const noexcept
{
	return _what.c_str();
}

/// \return Return _where
/// \brief Return where the exception happened
char const *Babel::Exception::where() const noexcept
{
	return _where.c_str();
}

/// \param exception : exception to be displayed on screen
/// \brief Display the exception given in a specific way
void Babel::Exception::displayException(const Babel::Exception &exception)
{
	std::cerr << "An exception is caught : " << exception.what() << " at " << exception.where() << std::endl;
}