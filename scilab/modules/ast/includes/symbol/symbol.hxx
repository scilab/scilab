/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

/**
 ** \file symbol.hh
 ** \brief Define class Symbol.
 */

#ifndef SYMBOL_HH
#define SYMBOL_HH

#ifdef _MSC_VER
#pragma warning (disable : 4251)
#endif
#include <set>
#include <list>
#include <string>
#include <iostream>

extern "C"
{
#include "dynlib_ast.h"
}

#define SCOPE_ALL       -1
#define SCOPE_GATEWAY   0
#define SCOPE_CONSOLE   1

namespace symbol
{

/** \brief Define class Symbol.
 **
 ** Map any wstring to a unique reference.
 ** This allows to avoid an "strcmp()" style comparison of strings:
 ** reference comparison is much faster.
 */
class EXTERN_AST Symbol
{
    /** \brief Define the type "set of wstring". */
    typedef std::set < std::wstring> string_set_type;
    /** \brief Define the type used for the size of wstring set. */
    typedef string_set_type::size_type size_type;

    /** \name Ctor & Dtor.
     ** \{ */
public:
    /** \brief Construct a Symbol (explicit).
     ** \param s referenced wstring */
    explicit Symbol (const std::wstring &s);
    /** \} */


    /** \name Accessors.
     ** \{ */
public:
    /** \brief Return the wstring referenced by this Symbol. */
    const std::wstring& getName () const;
    /** \brief Return the number of referenced strings. */
    static size_type getSize ();
    /** \brief Return all symbols. */
    static wchar_t** getAll ();
    /** \} */


    /** \name Operators.
     ** \{ */
public:
    /** \brief Compare two Symbol for equality.
     ** \param rhs Symbol to compare with. */
    bool operator== (const Symbol &rhs) const;
    /** \brief Compare two Symbol for inequality.
     ** \param rhs Symbol to compare with. */
    bool operator!= (const Symbol &rhs) const;
    /** \brief Compare two Symbol for order.
     ** \param rhs Symbol to compare with. */
    bool operator<  (const Symbol &rhs) const;
    /** \} */

    const string_set_type::const_iterator getNode() const
    {
        return _set_node;
    }

private:
    /** \brief Static (global to all instance of Symbol) strings container. */
    static string_set_type _set;
    /** \brief Pointer to the node containing the unique referenced wstring. */
    const string_set_type::const_iterator _set_node;
};

/** \brief Intercept output stream redirection.
 ** \param ostr the destination output stream
 ** \param the a reference to the symbol to redirect
 */
EXTERN_AST std::wostream& operator<< (std::wostream &ostr, const Symbol &the);

/** Typedef for the list of Symbol */
typedef std::list<const Symbol*> symbols_t;
}

namespace std
{
template<>
struct hash<symbol::Symbol>
{
    inline size_t operator()(const symbol::Symbol & sym) const
    {
        return std::hash<std::wstring>()(sym.getName());
    }
};
} // namespace std

#endif // !SYMBOL_HH
