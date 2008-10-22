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
 ** \file anydecs.hxx
 ** Define the Any Decs class.
 */

#ifndef AST_ANYDECS_HXX
# define AST_ANYDECS_HXX

# include <list>

# include "decs.hxx"

# include "vardec.hxx"
# include "typedec.hxx"
# include "functiondec.hxx"

namespace ast
{

  template < typename D >
  /** \brief Abstract a "list of D-declarations" node. */
  class AnyDecs : public Decs
  {
  public:
    /** \brief Define shortand type for list of D-declarations. */
    typedef std::list< D* > Ds;

    /** \name Ctor & dtor.
     ** \{ */
  public:
    /** \brief Construct an Any Decs node.
     ** \param location scanner position informations */
    AnyDecs (const Location& location) :
      Decs (location),
      _decs (new Ds ())
    {
    }
    /** \brief Construct an Any Decs node with a list of D-declarations.
     ** \param location scanner position informations
     ** \param decs list of D-declarations */
    AnyDecs (const Location& location, Ds& decs) :
      Decs (location),
      _decs (&decs)
    {
    }

    /** \brief Destroys an AnyDecs node.
     **
     ** Free list and its content. */
    ~AnyDecs ()
    {
      for (typename Ds::iterator i = _decs->begin(); i != _decs->end(); ++i)
	delete *i;
      delete _decs;
    }
    /** \} */


    /** \name Visitors entry point.
     ** \{ */
  public:
    /** \brief Accept a const visitor \a v. */
    virtual void accept (Visitor& v)
    {
      v.visit (*this);
    }
    /** \brief Accept a non-const visitor \a v. */
    virtual void accept (ConstVisitor& v) const
    {
      v.visit (*this);
    }
    /** \} */


    /** \name Accessors.
     ** \{ */
  public:
    /** \brief Push a D-declaration into the list.
     ** \param d declaration to push */
    AnyDecs< D >& push_back (D& d)
    {
      location_set (location_get () + d.location_get ());
      _decs->push_back (&d);
      return *this;
    }

    /** \brief Access to list of D-declarations (read and write). */
    Ds& decs_get ()
    {
      return *_decs;
    }
    /** \brief Access to list of D-declarations (read only). */
    const Ds& decs_get () const
    {
      return *_decs;
    }
    /** \} */

  private:
    Ds* _decs;
  };

  /** \brief Define shortand type for Variable declarations. */
  typedef AnyDecs< VarDec > VarDecs;
  /** \brief Define shortand type for Type declarations. */
  typedef AnyDecs< TypeDec > TypeDecs;
  /** \brief Define shortand type for Function declarations. */
  typedef AnyDecs< FunctionDec > FunctionDecs;

} // namespace ast

#endif // !AST_ANYDECS_HXX
