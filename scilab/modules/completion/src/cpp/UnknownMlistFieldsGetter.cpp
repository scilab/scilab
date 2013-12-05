/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "FieldsManager.hxx"
#include "UnknownMlistFieldsGetter.hxx"

extern "C"
{
#include "freeArrayOfString.h"
#include "api_scilab.h"
#include "MALLOC.h"
}

namespace org_modules_completion
{

const char ** UnknownMlistFieldsGetter::getFieldsName(const std::string & typeName, int * mlist, char ** fieldPath, const int fieldPathLen, int * fieldsSize) const
{
    return getFieldsName(mlist, fieldPath + 1, fieldPathLen - 1, fieldsSize);
}

const char ** UnknownMlistFieldsGetter::getFieldsName(int * mlist, char ** fieldPath, const int fieldPathLen, int * fieldsSize)
{
    int rows;
    int cols;
    int rc;
    int * piLen = 0;
    char ** pstData = 0;
    int nbItem;
    int * fieldsAddr = 0;
    SciErr sciErr;
    const char ** fields = 0;

    *fieldsSize = 0;

    sciErr = getListItemNumber(pvApiCtx, mlist, &nbItem);
    if (sciErr.iErr)
    {
        return 0;
    }

    if (nbItem == 0)
    {
        return 0;
    }

    sciErr = getListItemAddress(pvApiCtx, mlist, 1, &fieldsAddr);
    if (sciErr.iErr)
    {
        return 0;
    }

    if (getAllocatedMatrixOfString(pvApiCtx, fieldsAddr, &rows, &cols, &pstData))
    {
        return 0;
    }

    rc = rows * cols;
    if (rc == 1)
    {
        freeArrayOfString(pstData, rc);
        return 0;
    }

    if (fieldPathLen == 0)
    {
        *fieldsSize = rc - 1;
        fields = (const char **)MALLOC(sizeof(char *) **fieldsSize);
        memcpy(fields, pstData + 1, sizeof(char *) **fieldsSize);
        FREE(pstData[0]);
        FREE(pstData);

        return fields;
    }

    for (int i = 1; i < rc; i++)
    {
        if (!strcmp(pstData[i], fieldPath[0]))
        {
            freeArrayOfString(pstData, rc);
            int * itemAddr = 0;
            sciErr = getListItemAddress(pvApiCtx, mlist, i + 1, &itemAddr);
            if (sciErr.iErr)
            {
                return 0;
            }

            return FieldsManager::getFields(itemAddr, fieldPath, fieldPathLen, fieldsSize);
        }
    }

    return 0;
}
}
