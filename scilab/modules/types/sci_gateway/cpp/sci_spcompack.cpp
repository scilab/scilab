/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bernard HUGUENEY
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include "alltypes.hxx"
#include "sparse.hxx"
#include "funcmanager.hxx"
extern "C" {
#include "charEncoding.h"
#include "Scierror.h"
#include "localization.h"
}

namespace
{
// C++ code straight from spcompack.f
template<typename XlindxIt, typename LindxIt, typename XlnzIt, typename AdjncyIt>
void spcompack(int neqns, int nsuper, int nsub, int nnz, XlindxIt xlindx
                      , LindxIt lindx, XlnzIt xlnz, AdjncyIt adjncy)
{
    typedef typename std::iterator_traits<AdjncyIt>::difference_type AdjDiff_t;
    typedef typename std::iterator_traits<LindxIt>::difference_type LindxDiff_t;
    std::copy(lindx, lindx+nsub, adjncy);
    std::size_t i(1), j(1);
    for(i= j= 1; (j <= neqns) && (i != nsuper+1); ++j, ++i){
        if( !(((*(xlnz+j) - *(xlnz+j-1)) == (*(xlindx+i) - *(xlindx+i-1)))
              && (*(adjncy+ static_cast<AdjDiff_t>(*(xlnz+j-1))-1) == j)))
        {
            std::size_t const l(*(xlindx+nsuper)- *(xlindx+i-1)+(*(xlnz+j)-*(xlnz+j-1)));
            LindxIt const tmp(lindx+static_cast<LindxDiff_t>(*(xlindx+i-1)-(*(xlnz+j)- *(xlnz+j-1)))-1);
            std::copy(tmp, tmp+l
                      , adjncy+static_cast<AdjDiff_t>(*(xlnz+j-1))-1);
            --i;
        }
    }
    if(i == nsuper+1)
    {
        int const k(*(xlnz+neqns)- *(xlnz+j-1));
        i= 1;
        int ii= 1;
        while(i <= k)
        {
            for(j=1; j <= ii; ++j, ++i)
            {
                *(adjncy + static_cast<AdjDiff_t>(*(xlnz+neqns))-i-1) = neqns-j+1;
            }
            ++ii;
        }
    }
}
}

using namespace types;

//adjncy=spcompack(xadj,xlindx,lindx)
Function::ReturnValue sci_spcompack(typed_list &in, int nbRes, typed_list &out)
{
    if ( in.size() != 3 )
    {
        ScierrorW(999, _W("%ls: Wrong number of input argument(s): %d expected.\n"), L"spcompack", 3);
        return Function::Error;
    }
    for(std::size_t i(0); i!= 3; ++i)
    {
        /* Scilab <6 does not enforce vector args :( */
        if( in[i]->getType() != InternalType::RealDouble )
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: Real vector expected.\n"), L"spcompack", i+1);
            return Function::Error;
        }
    }
    if(nbRes > 1)
    {
        ScierrorW(999, _W("%ls: Wrong number of output arguments: %d expected.\n"), L"spcompack", 1);
        return Function::Error;
    }
    double const*const xadj(in[0]->getAsDouble()->real_get());
    std::size_t const nEqns(in[0]->getAsDouble()->size_get());
    double const*const xlindx(in[1]->getAsDouble()->real_get());
    std::size_t const nbSub(in[1]->getAsDouble()->size_get());
    double const*const lindx(in[2]->getAsDouble()->real_get());
    std::size_t const nSuper(in[2]->getAsDouble()->size_get());

    std::size_t const nnz(xadj[nEqns-1]-1);
    types::Double* const pAdjncy(new types::Double(nnz, 1));
    double* const adjncy(pAdjncy->real_get());
    spcompack(nEqns -1, nbSub-1,  nSuper-1, nnz-1
              , xlindx, lindx, xadj, adjncy);
    out.push_back(pAdjncy);
    return Function::OK;
}
