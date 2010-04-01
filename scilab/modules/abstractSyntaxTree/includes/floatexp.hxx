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

#ifndef AST_FLOATEXP_HXX
# define AST_FLOATEXP_HXX

# include "constexp.hxx"

namespace ast
{
    /** \brief Abstract an Float Expression node.
     **
     ** \b Example:  42.51 */
    class FloatExp : public ConstExp
    {
    public:
        FloatExp (const Location& location, float value) :
            ConstExp (location),
            _value (value)
            {
            }
        /** \brief Destroy an Float Expression node.
         **
         ** Delete size et init (exp) (see constructor). */
        virtual ~FloatExp ()
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
        float value_get() const
            {
                return _value;
            }
        /** \} */

    protected:
        float     _value;
    };

} // namespace ast
#endif
