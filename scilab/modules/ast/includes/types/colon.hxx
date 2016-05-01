/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

// This code is separated in colon.hxx
// but will be inlined in arrayof.hxx
//
// If you need additionnal headers, please add it in arrayof.hxx

//#ifndef __ARRAYOF_HXX__
//    #error This file must only be include by arrayof.hxx
//#endif

#ifndef __COLON_HXX__
#define __COLON_HXX__

#include "implicitlist.hxx"
#include "double.hxx"
#include "polynom.hxx"

namespace types
{
class EXTERN_AST Colon : public ImplicitList
{
public :

    Colon() : ImplicitList()
    {
        setStart(new Double(1));
        setStep(new Double(1));
        setEnd(Polynom::Dollar());
        compute();
    }

    Colon* clone()
    {
        return new Colon();
    }

    InternalType::ScilabType getType(void)
    {
        return ScilabColon;
    }

    inline ScilabId getId(void)
    {
        return IdColon;
    }

    bool isColon()
    {
        return true;
    }

};
}

#endif /* !__COLON_HXX__ */
