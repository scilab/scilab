/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 * Please note that piece of code will be rewrited for the Scilab 6 family
 * However, the API (profile of the functions in the header files) will be
 * still available and supported in Scilab 6.
 */

#include "gatewaystruct.hxx"
#include "sciMpiComm.hxx"

extern "C"
{
#include "getOptionalComm.h"
#include "api_scilab.h"
}

MPI_Comm getOptionalComm(void* _pvApiCtx)
{
    types::GatewayStruct* pStr = (types::GatewayStruct*)_pvApiCtx;
    types::optional_list opt = *pStr->m_pOpt;

    for (const auto & o : opt)
    {
        if (o.first != L"comm")
        {
            continue;
        }

        if (o.second->isUserType() == false || o.second->getAs<types::UserType>()->getTypeStr() != L"SciMpiComm")
        {
            return NULL;
        }

        SciMpiComm* sciComm = (SciMpiComm*)(o.second->getAs<types::UserType>());
        return sciComm->getComm();
    }

    return MPI_COMM_WORLD;
}
