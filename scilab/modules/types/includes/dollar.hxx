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

#ifndef __DOLLAR_HXX__
#define __DOLLAR_HXX__

#include "polynom.hxx"
#include "dynlib_types.h"
//#include "arrayof.hxx"

namespace types
{
class TYPES_IMPEXP Dollar : public Polynom
{
public :

    Dollar()
    {
        int piDims[2]   = {1, 1};
        m_szVarName	    = L"$";
        m_bComplex	    = false;
        SinglePoly** pPoly    = NULL;
        create(piDims, 2, &pPoly, NULL);

        Double dblCoef(1, 2);
        dblCoef.set(0, 0, 0);
        dblCoef.set(0, 1, 1.0);
        setCoef(0, &dblCoef);
    }

    Dollar* clone()
    {
        return new Dollar();
    }

    InternalType::RealType getType()
    {
        return RealDollar;
    }

    bool isDollar()
    {
        return true;
    }
};
}
#endif /* !__DOLLAR_HXX__ */
