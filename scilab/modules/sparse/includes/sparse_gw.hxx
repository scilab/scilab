/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __SPARSE_GW_HXX__
#define __SPARSE_GW_HXX__


#include "dynlib_sparse_gw.h"

#include "cpp_gateway_prototype.hxx"

class SparseModule
{
private :
    SparseModule() {};
    ~SparseModule() {};

public :
    SPARSE_GW_IMPEXP static int Load();
};

CPP_GATEWAY_PROTOTYPE(sci_adj2sp);
CPP_GATEWAY_PROTOTYPE(sci_nnz);
CPP_GATEWAY_PROTOTYPE(sci_sp2adj);
CPP_GATEWAY_PROTOTYPE(sci_sparse);
CPP_GATEWAY_PROTOTYPE(sci_spcompack);
CPP_GATEWAY_PROTOTYPE(sci_spget);
CPP_GATEWAY_PROTOTYPE(sci_spzeros);
CPP_GATEWAY_PROTOTYPE(sci_spones);
CPP_GATEWAY_PROTOTYPE(sci_full);

#endif /* !__SPARSE_GW_HXX__ */
