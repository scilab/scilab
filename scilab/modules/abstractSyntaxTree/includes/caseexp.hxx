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

#ifndef __AST_CASE_EXP_HXX__
#define __AST_CASE_EXP_HXX__

#include "exp.hxx"
#include "seqexp.hxx"

namespace ast {
  class CaseExp : public ControlExp
  {
  public :
    CaseExp(const Location& location,
	    Exp& test,
	    SeqExp& body) :
      ControlExp (location),
      _test (&test),
      _body (&body)
    {
    }
    
    ~CaseExp()
    {
      delete _test;
      delete _body;
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

  public :
	Exp* test_get() const { return _test; }
	SeqExp* body_get() const { return _body; }

  private :
    Exp* _test;
    SeqExp *_body;
  };

  /** \brief Define a shorthand for list of CaseExp* manipulation. */
  typedef std::list<CaseExp *> cases_t;
}

#endif /* !__AST_CASE_EXP_HXX__ */
