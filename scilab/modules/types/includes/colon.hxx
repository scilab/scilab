/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
#include "dollar.hxx"
#include "dynlib_types.h"

namespace types
{
class TYPES_IMPEXP Colon : public ImplicitList
{
public :

    Colon() : ImplicitList()
    {
        setStart(new Double(1));
        setStep(new Double(1));
        setEnd(new Dollar());
        compute();
    }

    Colon* clone()
    {
        return new Colon();
    }

    InternalType::RealType getType(void)
    {
        return RealColon;
    }

    bool isColon()
    {
        return true;
    }

};
}

#endif /* !__COLON_HXX__ */
