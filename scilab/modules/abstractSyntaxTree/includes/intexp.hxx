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

#ifndef AST_INTEXP_HXX
# define AST_INTEXP_HXX

#include "constexp.hxx"

namespace ast
{
    /** \brief Abstract an Int Expression node.
     **
     ** \b Example:  25 */
    class IntExp : public ConstExp
    {
    public :
        /** \brief Precision qualifier */
        enum Prec
        {
            /** \brief 8-bit Integer */	 _8_,
            /** \brief 16-bit Integer */ _16_,
            /** \brief 32-bit Integer */ _32_,
            /** \brief 64-bit Integer */ _64_
        };
    public:
        IntExp (const Location& location, Prec prec, int value) :
            ConstExp (location),
            _prec (prec),
            _value (value)
            {
            }
        
        IntExp (const Location& location, int value) :
            ConstExp (location),
            _prec (_32_),
            _value (value)
            {
            }

        /** \brief Destroy an Int Expression node.
         **
         ** Delete size et init (exp) (see constructor). */
        virtual ~IntExp ()
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
        /** \brief Return the value */
        int value_get() const
            {
                return _value;
            }
        
        Prec prec_get() const
            {
                return _prec;
            }
        /** \} */
    
    protected:
        Prec	_prec;
        int		_value;
    };

} // namespace ast
#endif
