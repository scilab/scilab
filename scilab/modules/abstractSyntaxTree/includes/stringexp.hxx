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

#ifndef AST_STRINGEXP_HXX
# define AST_STRINGEXP_HXX

# include "constexp.hxx"

namespace ast
{
  /** \brief Abstract an string Expression node.
   **
   ** \b Example: string*/
  class StringExp : public ConstExp
  {
  public:
    StringExp (const Location& location,
	       std::wstring value) :
      ConstExp (location),
      _value (value)
    {
    }
    /** \brief Destroy an string Exp node.
     **
     ** Delete value (see constructor). */
    virtual ~StringExp ()
    {
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
    /** \brief Return the name of the type name (read only). */
    const std::wstring value_get () const
    {
      return _value;
    }
    /** \} */
  protected:
    const std::wstring _value;
  };

} // namespace ast

#endif // !AST_STRINGEXP_HXX
