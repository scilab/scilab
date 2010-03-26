/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/**
 ** \file ast.hxx
 ** Define the Abstract Syntax Tree mother class.
 */

#ifndef AST_AST_HXX
# define AST_AST_HXX

# include "location.hxx"
# include "visitor.hxx"
#include "timer.hxx"

#ifdef _MSC_VER
	#if AST_EXPORTS
		#define EXTERN_AST __declspec (dllexport)
	#else
		#define EXTERN_AST __declspec (dllimport)
	#endif
#else
	#define EXTERN_AST 
#endif

namespace ast
{

  /** \brief Abstract an Abstract Syntax Tree node. */
  class EXTERN_AST Ast
  {
    /** \name Ctor & dtor.
     ** \{ */
  public:
    /** \brief Construct an Ast node.
     ** \param location scanner position informations */
    Ast (const Location& location) :
      _location (location)
    {
    }
    /** \brief Destroys an Ast node. */
    virtual ~Ast ()
    {
    }
    /** \} */

    /** \name Accessors.
     ** \{ */
  public:
    /** \brief Set scanner position informations.
     ** \param location scanner position informations
     **
     ** Return a reference to this.
     */
    Ast& location_set (const Location& location)
    {
      _location = location;
      return *this;
    }
    /** \brief Get scanner position informations stored. */
    const Location& location_get () const
    {
      return _location;
    }
    /** \} */

    /** \name Visitors entry point.
     ** \{ */
  public:
    /** \brief Accept a const visitor \a v. */
    virtual void accept (ConstVisitor& v) const = 0;
    /** \brief Accept a non const visitor \a v. */
    virtual void accept (Visitor& v) = 0;
    /** \} */

		Timer& timer_get(){return _timer;}

  private:
    /** \brief Construct an Ast by copy. */
    Ast (const Ast&);
    /** \brief Assign an Ast to this. */
    Ast& operator= (const Ast&);

    /** \brief Scanner position informations. */
		Timer _timer;
    Location _location;
  };

} // namespace ast

#endif // !AST_AST_HXX
