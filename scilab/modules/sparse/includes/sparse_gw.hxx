/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
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
    SPARSE_GW_IMPEXP static int Unload()
    {
        return 1;
    }
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
CPP_GATEWAY_PROTOTYPE(sci_ordmmd);
CPP_GATEWAY_PROTOTYPE(sci_lufact);
CPP_GATEWAY_PROTOTYPE(sci_lusolve);
CPP_GATEWAY_PROTOTYPE(sci_luget);
CPP_GATEWAY_PROTOTYPE(sci_ludel);
CPP_GATEWAY_PROTOTYPE(sci_spchol);
CPP_GATEWAY_PROTOTYPE(sci_sfinit);
CPP_GATEWAY_PROTOTYPE(sci_symfcti);
CPP_GATEWAY_PROTOTYPE(sci_bfinit);
CPP_GATEWAY_PROTOTYPE(sci_inpnv);
CPP_GATEWAY_PROTOTYPE(sci_blkfc1i);

#endif /* !__SPARSE_GW_HXX__ */
