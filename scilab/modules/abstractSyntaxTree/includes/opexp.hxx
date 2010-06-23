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

#ifndef AST_OPEXP_HXX
# define AST_OPEXP_HXX

# include <assert.h>
# include "mathexp.hxx"

namespace ast
{

  /** \brief Abstract an Operation Expression node.
   **
   ** \b Example: 77 * 27 */
  class OpExp : public MathExp
  {
  public:
    /** \brief Operator qualifier */
    enum Oper
    {
      // Arithmetics.
      /** \brief "+" */			plus,
      /** \brief "-" */			minus,
      /** \brief "*" */			times,
      /** \brief "/" */			divide,
      /** \brief "\" */			rdivide,
      /** \brief "**" or "^" */		power,

      // Element Ways.
      /** \brief ".*" */		dottimes,
      /** \brief "./" */		dotdivide,
      /** \brief ".\" */		dotrdivide,
      /** \brief ".^" */		dotpower,

      // Kroneckers
      /** \brief ".*." */		krontimes,
      /** \brief "./." */		krondivide,
      /** \brief ".\." */		kronrdivide,

      // Control
      // FIXME : What the hell is this ???
      /** \brief "*." */		controltimes,
      /** \brief "/." */		controldivide,
      /** \brief "\." */		controlrdivide,

      // Comparison.
      /** \brief "==" */		eq,
      /** \brief "<>" or "~=" */	ne,
      /** \brief "<" */			lt,
      /** \brief "<=" */		le,
      /** \brief "<" */			gt,
      /** \brief ">=" */		ge,

      // Logical operators
      /** \brief "&" */		logicalAnd,
      /** \brief "|" */		logicalOr,
      /** \brief "&&" */	logicalShortCutAnd,
      /** \brief "||" */	logicalShortCutOr
    };

    /** \brief Operation Expression kind.
     **
     ** Specify the result of Operation Expression (set at type checking). */
    enum Kind
    {
      /** \brief Invalid kind */
      invalid_kind,

      // Scalar values
      /** \brief Boolean result */
      bool_kind,
      /** \brief String result */
      string_kind,
      /** \brief Integer result */
      integer_kind,
      /** \brief Float result */
      float_kind,
      /** \brief Double result */
      double_kind,
      /** \brief Float Complex result */
      float_complex_kind,
      /** \brief Double Complex result */
      double_complex_kind,

      // Matrix values
      /** \brief Boolean Matrix result */
      bool_matrix_kind,
      /** \brief String Matrix result */
      string_matrix_kind,
      /** \brief Integer Matrix result */
      integer_matrix_kind,
      /** \brief Float Matrix result */
      float_matrix_kind,
      /** \brief Double Matrix result */
      double_matrix_kind,
      /** \brief Float Complex Matrix result */
      float_complex_matrix_kind,
      /** \brief Double Complex Matrix result */
      double_complex_matrix_kind,

      /** \brief Heterogeneous Matrix result */
      matrix_kind
    };


    /** \name Ctor & dtor.
     ** \{ */
  public:
    /** \brief Construct an Operation Expression node.
     ** \param location scanner position informations
     ** \param left left expression of the operator
     ** \param oper operator description
     ** \param right right expression of the operator
     **
     ** \b Example: 77 * 27
     ** \li "77" is the left expression
     ** \li "*" is the operator
     ** \li "27" is the right expression
     */
    OpExp (const Location& location,
	   Exp& left, Oper oper, Exp& right):
      MathExp (location),
      _left (&left),
      _oper (oper),
      _right (&right),
      _kind (invalid_kind)
    {
    }

    /** \brief Destroy a Operation Expression node.
     **
     ** Delete left and right, see constructor. */
    virtual ~OpExp ()
    {
			delete _left;
			delete _right;
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


    /** \name Setters.
     ** \{ */
  public :
    virtual void left_set(Exp& left)
    {
      _left = &left;
    }
    /** \} */


    /** \name Accessors.
     ** \{ */
  public:
    /** \brief Return the left expression of the operation (read only) */
    const Exp& left_get () const
    {
      return *_left;
    }
    /** \brief Return the left expression of the operation (read and write) */
    Exp& left_get ()
    {
      return *_left;
    }

    /** \brief Return the operator description (read only) */
    Oper oper_get () const
    {
      return _oper;
    }

		/** \brief Return the right expression of the operation (read only) */
    const Exp& right_get () const
    {
      return *_right;
    }
    /** \brief Return the right expression of the operation (read and write) */
    Exp& right_get ()
    {
      return *_right;
    }

    /** \brief Return the kind of the Op Expression (read only) \see Kind. */
    Kind kind_get (void) const
    {
      assert (_kind != invalid_kind);
      return _kind;
    }
    /** \brief Set the kind of the Op Expression \see Kind. */
    void kind_set (Kind kind)
    {
      this->_kind = kind;
    }
    /** \} */

  protected:
    /** \brief Left expression of the operation. */
    Exp* _left;
    /** \brief Operator. */
    Oper _oper;
    /** \brief Right expression of the operation. */
    Exp* _right;

    /** \brief Result type of the operation. */
    Kind _kind;
  };

} // namespace ast

#endif // !AST_OPEXP_HXX
