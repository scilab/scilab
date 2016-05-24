/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Allan CORNET
* Copyright (C) 2009 - DIGITEO - Allan CORNET
* ...
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
#include "fileio_gw.hxx"
#include "string.hxx"
#include "double.hxx"
#include "file.hxx"
#include "function.hxx"

extern "C"
{
#include "mget.h"
#include "localization.h"
#include "Scierror.h"
#include "charEncoding.h"
#include "os_string.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_mget(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    char* pstType   = os_strdup("l");//default type value : long
    int iSize       = 0;
    int iFile       = -1; //default file : last opened file
    int iErr        = 0;

    if (in.size() < 1 || in.size() > 3)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "mget", 1, 3);
        FREE(pstType);
        return types::Function::Error;
    }

    //check parameter 1
    if (in[0]->isDouble() == false || in[0]->getAs<types::Double>()->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A positive integer value expected.\n"), "mget", 1);
        FREE(pstType);
        return types::Function::Error;
    }

    types::Double* pDoubleTest = in[0]->getAs<types::Double>();
    if ((pDoubleTest->get(0) != (int)pDoubleTest->get(0)) || (pDoubleTest->get(0) < 0))
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: A positive integer value expected.\n"), "mget", 1);
        FREE(pstType);
        return types::Function::Error;
    }

    iSize = static_cast<int>(in[0]->getAs<types::Double>()->get(0));

    if (in.size() >= 2)
    {
        //export format
        if (in[1]->isString() == false || in[1]->getAs<types::String>()->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "mget", 2);
            FREE(pstType);
            return types::Function::Error;
        }

        FREE(pstType);
        pstType = wide_string_to_UTF8(in[1]->getAs<types::String>()->get(0));
    }

    if (in.size() == 3)
    {
        if (in[2]->isDouble() == false || in[2]->getAs<types::Double>()->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A integer expected.\n"), "mget", 3);
            FREE(pstType);
            return types::Function::Error;
        }

        iFile = static_cast<int>(in[2]->getAs<types::Double>()->get(0));
    }

    switch (iFile)
    {
        case 0: // stderr
        case 6: // stdout
            FREE(pstType);
            Scierror(999, _("%s: Wrong file descriptor: %d.\n"), "mget", iFile);
            return types::Function::Error;
    }

    types::File* pFile = FileManager::getFile(iFile);
    // file opened with fortran open function
    if (pFile == NULL || pFile->getFileType() == 1)
    {
        FREE(pstType);
        Scierror(999, _("%s: Wrong file descriptor: %d.\n"), "mget", iFile);
        return types::Function::Error;
    }


    types::Double* pD = new types::Double(1, iSize);
    double* pData = pD->get();
    C2F(mget)(&iFile, pData, &iSize, pstType, &iErr);
    FREE(pstType);
    if (iErr > 0)
    {
        return types::Function::Error;
    }

    if (iErr < 0) //no error
    {
        int iNewSize = (-iErr) - 1;
        if (iNewSize < iSize)
        {
            //read data are smaller then excepted size
            types::Double* pNewD = new types::Double(1, iNewSize);
            double* pNewData = pNewD->getReal();
            for (int i = 0 ; i < iNewSize ; i++)
            {
                pNewData[i] = pData[i];
            }

            delete pD;
            pD = pNewD;
        }
    }
    out.push_back(pD);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
