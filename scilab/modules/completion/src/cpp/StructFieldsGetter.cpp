/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
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

#include "FieldsManager.hxx"
#include "StructFieldsGetter.hxx"

extern "C"
{
#include "freeArrayOfString.h"
#include "api_scilab.h"
#include "sci_malloc.h"
}

namespace org_modules_completion
{

const char ** StructFieldsGetter::getFieldsName(const std::string & typeName, int * mlist, char ** fieldPath, const int fieldPathLen, int * fieldsSize) const
{
    return getFieldsName(mlist, fieldPath + 1, fieldPathLen - 1, fieldsSize);
}

const char ** StructFieldsGetter::getFieldsName(int * mlist, char ** fieldPath, const int fieldPathLen, int * fieldsSize)
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

    sciErr = getListItemNumber(NULL, mlist, &nbItem);
    if (sciErr.iErr)
    {
        return 0;
    }

    if (nbItem == 0)
    {
        return 0;
    }

    sciErr = getListItemAddress(NULL, mlist, 1, &fieldsAddr);
    if (sciErr.iErr)
    {
        return 0;
    }

    if (getAllocatedMatrixOfString(NULL, fieldsAddr, &rows, &cols, &pstData))
    {
        return 0;
    }

    rc = rows * cols;
    if (rc == 1 || rc == 2)
    {
        freeArrayOfString(pstData, rc);
        return 0;
    }

    if (fieldPathLen == 0)
    {
        *fieldsSize = rc - 2;
        fields = (const char **)MALLOC(sizeof(char *) **fieldsSize);
        memcpy(fields, pstData + 2, sizeof(char *) **fieldsSize);
        FREE(pstData[0]);
        FREE(pstData[1]);
        FREE(pstData);

        return fields;
    }

    for (int i = 2; i < rc; i++)
    {
        if (!strcmp(pstData[i], fieldPath[0]))
        {
            freeArrayOfString(pstData, rc);
            int * itemAddr = 0;
            sciErr = getListItemAddress(NULL, mlist, i + 1, &itemAddr);
            if (sciErr.iErr)
            {
                return 0;
            }

            return FieldsManager::getFields(itemAddr, fieldPath, fieldPathLen, fieldsSize);
        }
    }

    freeArrayOfString(pstData, rc);

    return 0;
}
}
