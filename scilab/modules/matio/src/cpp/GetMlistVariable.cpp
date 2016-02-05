/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Yann COLLETTE
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

#include "GetMatlabVariable.hxx"

extern "C"
{
#include <string.h>


#include "sci_types.h"
#include "api_scilab.h"
#include "freeArrayOfString.h"
#include "sci_malloc.h"
}

matvar_t *GetMlistVariable(void *pvApiCtx, int iVar, const char *name, int matfile_version, int * parent, int item_position)
{

    types::GatewayStruct* pGS = (types::GatewayStruct*)pvApiCtx;
    types::typed_list in = *pGS->m_pIn;

    if (in[iVar - 1]->isMList() == false)
    {
        Scierror(999, _("%s: Wrong type for argument %d: MList expected.\n"), "GetMlistVariable");
        return NULL;
    }

    types::MList* pMList = in[iVar - 1]->getAs<types::MList>();

    return GetMListMatVar(pMList, name, matfile_version);
}


matvar_t* GetMListMatVar(types::MList* pMist, const char* name, int matfile_version)
{
    char* pcName = wide_string_to_UTF8(pMist->getFieldNames()->get(0));
    Scierror(999, _("%s: Mlists of type %s can not be written to Matlab binary files.\n"), "GetMListMatVar", pcName);
    FREE(pcName);
    return NULL;
}
