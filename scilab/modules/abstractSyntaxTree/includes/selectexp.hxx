/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 * 
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 * 
 */

/**
 ** \file selectexp.hxx
 ** Define Select Expression class
 */

#ifndef __AST_SELECT_EXP_HXX__
#define __AST_SELECT_EXP_HXX__

#include "controlexp.hxx"
#include "exp.hxx"
#include "caseexp.hxx"
#include "seqexp.hxx"

namespace ast
{

  class SelectExp : public ControlExp
  {
  public :
    SelectExp(const Location& location,
			  Exp& select,
			  cases_t& cases, 
			  SeqExp& defaultCase) :
      ControlExp (location),
      _selectme (&select),
      _cases (&cases),
      _default (&defaultCase)
    {
    }

    SelectExp(const Location& location,
			  Exp& select,
			  cases_t& cases) : 
      ControlExp (location),
      _selectme (&select),
      _cases (&cases)
    {
      _default = NULL;
    }

    ~SelectExp()
    {
      delete _selectme;
      delete _cases;
      if (_default != NULL) 
		{
		  delete _default;
		}
    }

  public :
	Exp* select_get() const { return _selectme; }
	cases_t* cases_get() const { return _cases; }
	SeqExp* default_case_get() const { return _default; }

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

  private :
    Exp* _selectme;
    cases_t* _cases;
    SeqExp* _default;
  };

}


#endif /* !__AST_SELECT_EXP_HXX__ */
