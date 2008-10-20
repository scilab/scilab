/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/**
 ** \file symbol.hh
 ** \brief Define class Symbol.
 */

#ifndef SYMBOL_HH
# define SYMBOL_HH

#include <set>
#include <list>
#include <string>
#include <iostream>

namespace symbol
{

  /** \brief Define class Symbol.
   **
   ** Map any string to a unique reference.
   ** This allows to avoid an "strcmp()" style comparison of strings:
   ** reference comparison is much faster.
   */
  class Symbol
  {
    /** \brief Define the type "set of string". */
    typedef std::set < std::string > string_set_type;
    /** \brief Define the type used for the size of string set. */
    typedef string_set_type::size_type size_type;

    /** \name Ctor & Dtor.
     ** \{ */
  public:
    /** \brief Construct a Symbol (explicit).
     ** \param s referenced string */
    explicit Symbol (const std::string &s);
    /** \} */


    /** \name Accessors.
     ** \{ */
  public:
    /** \brief Return the string referenced by this Symbol. */
    const std::string& name_get () const;
    /** \brief Return the number of referenced strings. */
    static size_type map_size ();
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


  private:
    /** \brief Static (global to all instance of Symbol) strings container. */
    static string_set_type _set;
    /** \brief Pointer to the node containing the unique referenced string. */
    const string_set_type::const_iterator _set_node;
  };

  /** \brief Intercept output stream redirection.
   ** \param ostr the destination output stream
   ** \param the a reference to the symbol to redirect
   */
  std::ostream& operator<< (std::ostream &ostr, const Symbol &the);

  /** Typedef for the list of Symbol */
  typedef std::list<const Symbol*> symbols_t;
}

# include "symbol.hxx"

#endif // !SYMBOL_HH
