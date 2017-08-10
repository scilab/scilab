/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
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
/*--------------------------------------------------------------------------*/
#include "filemanager.hxx"

extern "C"
{
#include "mclose.h"
#include "delfile.h"
#include "core_math.h" /* Min Max */
#include "sciprint.h"
#include "localization.h"
#include "configvariable_interface.h"
#include "charEncoding.h"

    extern int C2F(clunit)(int* , const char*, int*, int);
}
/*--------------------------------------------------------------------------*/
int mcloseCurrentFile()
{
    return mclose(FileManager::getCurrentFile());
}

int mcloseAll()
{
    int iFileCount = FileManager::getFileMaxID();
    for (int i = iFileCount - 1 ; i >= 0 ; i--)
    {
        switch (i)
        {
            case 0: // stderr
            case 5: // stdin
            case 6: // stdout
                continue;
                break;
            default :
                if (FileManager::getFile(i) != NULL)
                {
                    //call mclose only for existing opened files.
                    int iRet = mclose(i);
                    if (iRet)
                    {
                        return iRet;
                    }
                }
        }
    }

    return 0;
}

int mclose(int _iID)
{
    types::File* pFile = FileManager::getFile(_iID);
    if (pFile != NULL)
    {
        if (pFile->getFileType() == 1)
        {
            int iFileNum = -_iID; // -_iID to close file _iID
            const char* stFilename = ""; // file will be closed by this num
            int iMode = 0; // not used in close mode
            return C2F(clunit)(&iFileNum, stFilename, &iMode, 1L);
        }
        else if (pFile->getFileType() == 2)
        {
            int iRet = fclose(pFile->getFiledesc());

            // this function previously called ferror on a just before fclosed FILE* that could lead to crash at exit, depending on libc implementation.
            if (iRet != 0)
            {
                return 1;
            }

            FileManager::deleteFile(_iID);
        }
    }
    else
    {
        if (getWarningMode())
        {
            sciprint(_("%s: Cannot close file whose descriptor is %d: File is not active.\n"), "mclose", _iID);
        }
    }
    return 0;
}

void C2F(mclose) (int *fd, double *res)
{
    int fd1 = -1;
    *res = 0.0;

    switch (*fd)
    {
        case ALL_FILES_DESCRIPTOR :
        {
            /* closing all opened files */
            mcloseAll();
        }
        break;
        default :
        {
            if (mclose(*fd))
            {
                *res = -1.0;
            }
        }
    }
}
/*--------------------------------------------------------------------------*/
