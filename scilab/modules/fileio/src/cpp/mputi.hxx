/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2017 - Scilab Enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __MPUTI_H__
#define __MPUTI_H__

#include <string.h>
#include "filemanager.hxx"
#include "mputi.hxx"

extern "C" {
#include "Scierror.h"
#include "localization.h"
#include "islittleendian.h"
#include "convert_tools.h"
}


template <typename T>
static T mputi_getIntVal(const T* in, int index)
{
    T ret = (T)in[index];
    return ret;
}

template <typename T>
int mputi(int fd, const T* _val, int size, const char* opt)
{
    int iType = 0;
    int iUnsigned = 0;
    int iEndian = 0;

    int iTypeLen = (int)strlen(opt);
    int i;

    types::File *pFile = FileManager::getFile(fd);
    if (!pFile || pFile->getFiledesc() == NULL)
    {
        Scierror(999, _("%s: No output file.\n"), "mputi");
        return 1;
    }

    if (iTypeLen == 1)
    {
        //type only
        iUnsigned = SIGNED;
        iType = checkType(opt[0]);
    }
    else if (iTypeLen == 2)
    {
        if (opt[0] == 'u')
        {
            //unsigned + type
            iUnsigned = UNSIGNED;
            iType = checkType(opt[1]);
        }
        else
        {
            //type + endian
            iUnsigned = SIGNED;
            iType = checkType(opt[0]);
            iEndian = checkEndian(opt[1]);
        }
    }
    else if (iTypeLen == 3)
    {
        if (opt[0] == 'u')
        {
            //unsigned + type
            iUnsigned = UNSIGNED;
            iType = checkType(opt[1]);
            iEndian = checkEndian(opt[2]);
        }
    }

    if (iEndian == 0)
    {
        //endian can be setting up by mopen call with flag swap
        if (pFile->getFileSwap())
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
        Scierror(999, _("%s: %s format not recognized.\n"), "mputi", opt);
        return 1;
    }

    switch (iType)
    {
        case TYPE_LONG_LONG:
            for (i = 0; i < size; i++)
            {
                unsigned long long val;
                val = (unsigned long long)mputi_getIntVal(_val, i);
                if (writeLongLong(val, pFile->getFiledesc(), iEndian))
                {
                    return 1;
                }
            }
            break;
        case TYPE_INT:
            for (i = 0; i < size; i++)
            {
                unsigned int val;
                val = (unsigned int)mputi_getIntVal(_val, i);
                if (writeInt(val, pFile->getFiledesc(), iEndian))
                {
                    return 1;
                }
            }
            break;
        case TYPE_SHORT:
            for (i = 0; i < size; i++)
            {
                unsigned short val;
                val = (unsigned short)mputi_getIntVal(_val, i);
                if (writeShort(val, pFile->getFiledesc(), iEndian))
                {
                    return 1;
                }
            }
            break;
        case TYPE_CHAR:
            for (i = 0; i < size; i++)
            {
                unsigned char val;
                val = (unsigned char)mputi_getIntVal(_val, i);
                if (writeChar(val, pFile->getFiledesc(), iEndian))
                {
                    return 1;
                }
            }
            break;
    }

    return 0;
}

#endif /* __MPUTI_H__ */
