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
 ** \file trycatchexp.hxx
 ** Define the Trycatchuence Expression class.
 */

#ifndef AST_TRYCATCHEXP_HXX
#define AST_TRYCATCHEXP_HXX

#include "controlexp.hxx"
#include "seqexp.hxx"

namespace ast
{

    /** \brief Abstract a Trycatchuence Expression node.
    **
    ** \b Example: try (print_int(2097); catch print("WipeOut")) */
    class TryCatchExp : public ControlExp
    {
        /** \name Ctor & dtor.
        ** \{ */
    public:
        /** \brief Construct a Trycatchuence Expression node.
        ** \param location scanner position informations
        ** \param body EXP LIST intruction
        */
        TryCatchExp (const Location& location,
            SeqExp& tried,
            SeqExp& catched) 
            : ControlExp (location),
            _tryme (&tried),
            _catchme (&catched)
        {
        }

        virtual ~TryCatchExp ()
        {
            delete _tryme;
            delete _catchme;
        }

        virtual TryCatchExp* clone()
        {
            Location* newloc = const_cast<Location*>(&location_get())->clone();
            return new TryCatchExp(*newloc, *try_get().clone(), *catch_get().clone());
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
        const SeqExp&	try_get() const
        {
            return *_tryme;
        }

        SeqExp&		try_get()
        {
            return *_tryme;
        }
        const SeqExp&	catch_get() const
        {
            return *_catchme;
        }

        SeqExp&		catch_get()
        {
            return *_catchme;
        }
        /** \} */


    protected:
        SeqExp* _tryme;
        SeqExp* _catchme;
    };

} // namespace ast

#endif /* !AST_TRYCATCHEXP_HXX */
