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

/**
 ** \file seqexp.hxx
 ** Define the Sequence Expression class.
 */

#ifndef __AST_SEQEXP_HXX__
# define __AST_SEQEXP_HXX__

# include "exp.hxx"
#include "functiondec.hxx"

namespace ast
{

  /** \brief Abstract a Sequence Expression node.
   **
   ** \b Example: (print_int(2097); print("WipeOut")) */
  class SeqExp : public Exp
  {
    /** \name Ctor & dtor.
     ** \{ */
  public:
    /** \brief Construct a Sequence Expression node.
     ** \param location scanner position informations
     ** \param body EXP LIST intruction
     */
    SeqExp (const Location& location,
	    std::list<Exp *>& l_body) :
      Exp (location),
      _l_body (&l_body)
    {
    }

    virtual ~SeqExp ()
    {
			std::list<Exp *>::const_iterator i;
			for(i = _l_body->begin() ; i != _l_body->end() ; i++)
			{
                FunctionDec* pDec = dynamic_cast<FunctionDec*>(*i);
                if(pDec)
                {//do not delete function declaration.
                    continue;
                }
				delete *i;
			}
      delete _l_body;
    }

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
    const std::list<Exp *>&	exps_get() const
    {
      return *_l_body;
    }

    std::list<Exp *>&	exps_get()
    {
      return *_l_body;
    }
    /** \} */


  protected:
    std::list<Exp *>* _l_body;
  };

} // namespace ast

#endif // __AST_SEQEXP_HXX__
