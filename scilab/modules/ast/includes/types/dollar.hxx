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

namespace types
{
class EXTERN_AST Dollar : public Polynom
{
public :

    Dollar()
    {
        int piDims[2] = {1, 1};
        m_szVarName   = L"$";
        SinglePoly** pPoly = NULL;
        create(piDims, 2, &pPoly, NULL);

        double* pdblCoef = NULL;
        SinglePoly* SPCoef = new SinglePoly(&pdblCoef, 1);
        pdblCoef[0] = 0;
        pdblCoef[1] = 1.0;

        m_pRealData[0] = SPCoef;
    }

    Dollar* clone()
    {
        return new Dollar();
    }

    inline ScilabType getType()
    {
        return ScilabDollar;
    }
    inline ScilabId getId()
    {
        return IdScalarPolynom;
    }

    bool isDollar()
    {
        return true;
    }
};
}
#endif /* !__DOLLAR_HXX__ */
