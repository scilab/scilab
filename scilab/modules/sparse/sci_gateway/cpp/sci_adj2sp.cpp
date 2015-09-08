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

#include <algorithm>
#include <iostream>

#include "sparse_gw.hxx"
#include "function.hxx"
#include "sparse.hxx"

extern "C"
{
#include "charEncoding.h"
#include "Scierror.h"
#include "localization.h"
}

using namespace types;

Function::ReturnValue sci_adj2sp(typed_list &in, int nbRes, typed_list &out)
{
    if (in.size() > 4 || in.size() < 3)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "adj2sp", 3, 4);
        return Function::Error;
    }
    if (nbRes != 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), "adj2sp", 1);
        return Function::Error;
    }

    for (std::size_t i = 0; i != 3; i++)
    {
        if (in[i]->isDouble() == false || in[i]->getAs<Double>()->isVector() == false)
        {
            Scierror(999, _("%s: Wrong type for argument %d: Real vector expected.\n"), "adj2sp", i + 1);
            return Function::Error;
        }
    }

    for (std::size_t i = 3 ; i != in.size() ; i++)
    {
        if (in[i]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for argument %d: Matrix expected.\n"), "adj2sp", i + 1);
            return Function::Error;
        }
    }

    types::Double* xadj     = in[0]->getAs<Double>();
    types::Double* adjncy   = in[1]->getAs<Double>();
    types::Double* anz      = in[2]->getAs<Double>();

    std::size_t n = xadj->getSize() - 1;
    size_t m = static_cast<size_t>(*std::max_element(adjncy->getReal(), adjncy->getReal() + adjncy->getSize()));

    if (in.size() == 4)
    {
        Double& dims = *(in[3]->getAs<Double>());
        if (dims.getRows() != 1 || dims.getCols() != 2)
        {
            Scierror(999, _("%s: Wrong size for input arguments %d: A vector of size %d expected.\n" ), "adj2sp", 4, 2);
            return Function::Error;
        }

        if (m > dims.getReal(0, 0) || n > dims.getReal(0, 1))
        {
            Scierror(999, _("%s: Incompatible input argument.\n"), "adj2sp");
            return Function::Error;
        }

        m = static_cast<size_t>(dims.getReal(0, 0));
        n = static_cast<size_t>(dims.getReal(0, 1));
    }

    out.push_back(new types::Sparse(*xadj, *adjncy, *anz, m, n));
    return Function::OK;
}
