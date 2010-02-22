/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef AST_CALLEXP_HXX
# define AST_CALLEXP_HXX

# include <list>
# include "exp.hxx"

namespace ast
{
  /*
  ** \brief Abstract a Call Expression node.
  **
  ** \b Example: foo(2097)
  */
  class CallExp : public Exp
  {
    // \brief Ctor & dtor.
  public:
    /*
    ** \brief Construct a Call Expression node.
    ** \param location scanner position informations
    ** \param name of the function
    ** \param list of the arguments
    */
    CallExp (const Location& location,
	     Exp& name,
	     std::list<Exp *>& args) :
      Exp (location),
      _name (&name),
      _args (&args)
    {
    }

    virtual ~CallExp ()
    {
			delete _name;
			std::list<Exp *>::const_iterator i;
			for(i = _args->begin() ; i != _args->end() ; i++)
			{
				delete *i;
			}
			delete _args;
    }


    // \brief Visitors entry point.
  public:
    // \brief Accept a const visitor
    virtual void	accept(Visitor& v)
    {
      v.visit (*this);
    }
    // \brief Accept a non-const visitor
    virtual void	accept(ConstVisitor& v) const
    {
      v.visit (*this);
    }


    // \brief Accessors.
  public:
    const Exp&	name_get() const
    {
      return *_name;
    }

    Exp&	name_get()
    {
      return *_name;
    }

    void name_set (Exp *name)
    {
      _name = name;
    }

    const std::list<Exp *>&	args_get() const
    {
      return *_args;
    }

    std::list<Exp *>&	args_get()
    {
      return *_args;
    }

  protected:
    Exp*		_name;
    std::list<Exp *>*	_args;
  };

} // namespace ast

#endif // !AST_CALLEXP_HXX
