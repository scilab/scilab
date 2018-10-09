/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2018 - Stéphane MOTTELET
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */
/*--------------------------------------------------------------------------*/
#include "interpolation_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "sparse.hxx"
#include <numeric>


extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "interpolation_functions.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_mesh2di(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 3)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "mesh2di", 3);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "mesh2di", 1);
        return types::Function::Error;
    }

    for (int i = 0; i < in.size(); i++)
    {
        if (!in[i]->isDouble() | in[i]->getAs<types::Double>()->isComplex())
        {
            Scierror(999, _("%s: Wrong type for argument #%d: Real matrix expected.\n"), "mesh2di", i);
            return types::Function::Error;
        }
    }

    types::Double *pDblx = in[0]->getAs<types::Double>();
    types::Double *pDbly = in[1]->getAs<types::Double>();

    if (pDblx->getSize() != pDblx->getSize())
    {
        Scierror(999, _("%s: Arguments #%d and #%d must have the same sizes.\n"), "mesh2di", 1,2);
        return types::Function::Error;
    }

    int iNbNodes = pDblx->getSize();

    if (iNbNodes < 3)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d : At least %d elements expected.\n"), "mesh2di", 1, 3);
        return types::Function::Error;
    }
    
    double* pdblx = pDblx->get();
    double* pdbly = pDbly->get();
    std::vector<int> bdy;

    // verify boundary specification
    types::Double* pDblBdy = in[2]->getAs<types::Double>();
    int iStart = -1;

    for (int i=0; i < pDblBdy->getSize(); i++)
    {
        double dblCurrNode = pDblBdy->get(i);
        if (dblCurrNode != std::round(dblCurrNode) || (int)dblCurrNode < 1 || (int)dblCurrNode > iNbNodes)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: incorrect element #%d\n"), "mesh2di", 3, i+1);
            return types::Function::Error;
        }
        // if (iStart < 0) next node is the first of a connected component
        // else if iCurrNode == iStart, close the current component (set iStart = -1)
        iStart = iStart < 0 ? (int)dblCurrNode : ((int)dblCurrNode == iStart ? -1 : iStart);
        bdy.push_back((int)dblCurrNode);
    }
    // iStart should be equal to -1 (all components closed)
    if (iStart != -1)
    {
        Scierror(999, _("%s: last connected component of boundary is not closed\n"), "mesh2di");
        return types::Function::Error;
    }

    // allocate working arrays
    int* piTri = NULL;
    int iNbTri = 0;
    int iErr = 0;
    int in4 = 4*iNbNodes-4;
    int in6 = 6*(2*iNbNodes-2);
    int* piWork = new int[in4];
    piTri = new int[3*in4];
    int iNbBdyNodes = bdy.size();
    
    // mesh2b wants xy=[x(:)';y(:)'] + a clone of it
    types::Double* pDblxy = new types::Double(2,iNbNodes);
    for (int i=0; i < iNbNodes; i++)
    {
         pDblxy->set(0,i,pDblx->get(i));
         pDblxy->set(1,i,pDbly->get(i));
    }
    types::Double* pDblxyClone = pDblxy->clone();

    C2F(mesh2b)(&iNbNodes, &in6, &in4, &iNbBdyNodes, pDblxy->get(), pDblxyClone->get(),
                 piTri, piWork, &bdy.front(), &iNbTri, &iErr);

    pDblxy->killMe();
    pDblxyClone->killMe();
    delete[] piWork;

    if (iErr)
    {
        std::string errStr[] = {
        _("%s: some points are identical.\n"),
        _("%s: some points are identical.\n"),
        _("%s: all points are aligned.\n"),
        _("%s: wrong boundary array.\n"),
        _("%s: crossed boundary.\n"),
        _("%s: wrong orientation of the boundary.\n"),
        _("%s: size limitation.\n"),
        _("%s: crossed boundary.\n"),
        _("%s: an interior point is on the boundary.\n"),
        _("%s: an interior point is too close to the boundary.\n"),
        _("%s: some points are identical or size limitation.\n")};

        Scierror(999, errStr[iErr-1].c_str(), "mesh2di");

        delete[] piTri;

        return types::Function::Error;
    }
    
    // fix empty triangles + eventual wrong orientation (should be counter clock-wise)
    types::Double* pDblTrianglesNodes = new types::Double(3,iNbTri);
    double* pdblTri = pDblTrianglesNodes->get();
    int iTriLin = 0;
    int iTriLinOrig = 0;
    int i0, i1, i2;

    for (int i = 0; i < iNbTri; i++)
    {
        i0 = piTri[iTriLinOrig++];
        i1 = piTri[iTriLinOrig++];
        i2 = piTri[iTriLinOrig++];
        if (i0>0) // triangle is not empty
        {
            pdblTri[iTriLin++] = (double)i0;
            // compute determinant of (i0->i1) and (i0->i2) vectors
            if ((pdblx[i1-1]-pdblx[i0-1])*(pdbly[i2-1]-pdbly[i0-1])-
                (pdblx[i2-1]-pdblx[i0-1])*(pdbly[i1-1]-pdbly[i0-1]) > 0.0)
            {
                pdblTri[iTriLin++] = (double)i1;
                pdblTri[iTriLin++] = (double)i2;
            }
            else
            {
                pdblTri[iTriLin++] = (double)i2;
                pdblTri[iTriLin++] = (double)i1;
            }
        }
    }
    delete[] piTri;
    pDblTrianglesNodes->resize(3,iTriLin/3);

    out.push_back(pDblTrianglesNodes);

    return types::Function::OK;
}

