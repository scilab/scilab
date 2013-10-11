/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA -
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

#include <string.h>

#include "mputi.h"
#include "sciprint.h"
#include "filesmanagement.h"
#include "localization.h"
#include "islittleendian.h"
#include "convert_tools.h"


void C2F(mputi)(int* _pF, long long* _pVal, int* _iSize, char* _iOpt, int* _iErr)
{
    int iType = 0;
    int iUnsigned = 0;
    int iEndian = 0;

    int iTypeLen = (int)strlen(_iOpt);
    int i;
    FILE *fa = NULL;

    unsigned char *RES_uc   = (unsigned char*)_pVal;
    unsigned short *RES_us  = (unsigned short*)_pVal;
    unsigned int *RES_ui    = (unsigned int*)_pVal;

    fa = GetFileOpenedInScilab(*_pF);
    if (fa == NULL)
    {
        sciprint(_("%s: No input file.\n"), "mputi");
        *_iErr = 1;
        return;
    }

    if (iTypeLen == 1)
    {
        //type only
        iUnsigned = SIGNED;
        iType = checkType(_iOpt[0]);
    }
    else if (iTypeLen == 2)
    {
        if (_iOpt[0] == 'u')
        {
            //unsigned + type
            iUnsigned = UNSIGNED;
            iType = checkType(_iOpt[1]);
        }
        else
        {
            //type + endian
            iUnsigned = SIGNED;
            iType = checkType(_iOpt[0]);
            iEndian = checkEndian(_iOpt[1]);
        }
    }
    else if (iTypeLen == 3)
    {
        if (_iOpt[0] == 'u')
        {
            //unsigned + type
            iUnsigned = UNSIGNED;
            iType = checkType(_iOpt[1]);
            iEndian = checkEndian(_iOpt[2]);
        }
    }

    if (iEndian == 0)
    {
        //endian can be setting up by mopen call with flag swap
        if (GetSwapStatus(*_pF))
        {
            iEndian = islittleendian() ? BIG_ENDIAN : LITTLE_ENDIAN;
        }
        else
        {
            iEndian = islittleendian() ? LITTLE_ENDIAN : BIG_ENDIAN;
        }
    }
    else
    {
        if (iEndian == LITTLE_ENDIAN)
        {
            iEndian = islittleendian() ? LITTLE_ENDIAN : BIG_ENDIAN;
        }
        else
        {
            iEndian = islittleendian() ? BIG_ENDIAN : LITTLE_ENDIAN;
        }
    }

    if (iType == 0 || iEndian == 0 || iUnsigned == 0)
    {
        sciprint(_("%s: %s format not recognized.\n"), "mputi", _iOpt);
        *_iErr = 1;
        return;
    }

    switch (iType)
    {
        case TYPE_LONG_LONG :
            for (i = 0; i < *_iSize ; i++)
            {
                unsigned long long val;
                val = *(_pVal + i);
                *_iErr = writeLongLong(val, fa, iEndian);
            }
            break;
        case TYPE_INT :
            for (i = 0; i < *_iSize ; i++)
            {
                unsigned int val;
                val = *(RES_ui + i);
                *_iErr = writeInt(val, fa, iEndian);
            }
            break;
        case TYPE_SHORT :
            for (i = 0; i < *_iSize ; i++)
            {
                unsigned short val;
                val = *(RES_us + i);
                *_iErr = writeShort(val, fa, iEndian);
            }
            break;
        case TYPE_CHAR:
            for (i = 0; i < *_iSize ; i++)
            {
                unsigned char val;
                val = *(RES_uc + i);
                *_iErr = writeChar(val, fa, iEndian);
            }
            break;
    }
}
