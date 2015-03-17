/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
* Copyright (C) 2015 - Scilab Enterprises - Sylvain GENIN
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <string.h>
#include "function.hxx"
#include "io_gw.hxx"
#include "string.hxx"
#include "double.hxx"
#include "int.hxx"
#include "filemanager.hxx"


extern "C"
{
#include "sci_malloc.h"
#include "Scierror.h"
#include "expandPathVariable.h"

    extern int C2F(clunit)(int*, char const*, int*, int);

    extern int C2F(readdoublefile)(int* ID, double* dat, int* m, int* n, int* err);
    extern int C2F(readdoublefileform)(int* ID, char* form, double* dat, int* m, int* n, int* err, int);
    extern int C2F(readdoublelinefile)(int* ID, double* dat, int* n, int* err);
    extern int C2F(readdoublelinefileform)(int* ID, char* form, double* dat, int* n, int* err);

    extern int C2F(readintfileform)(int* ID, char* form, int* dat, int* m, int* n, int* err, int);
    extern int C2F(readintlinefileform)(int* ID, char* form, int* dat, int* n, int* err);

    extern int C2F(readstringfile)(int* ID, char* form, char* dat, int* siz, int* err, int);
    extern int C2F(readstring)(char* form, char* dat, int* siz, int* err, int);

}

using namespace types;

int checkformatread(char* format);
template<typename T>
bool is_of_type(const std::string & Str);


/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_read(typed_list &in, int _iRetCount, typed_list &out)
{
    int iID = 0;
    int iAcces = 0;
    int iRhs = (int)in.size();
    char* pstFormat = NULL;

    types::InternalType::ScilabType itTypeOfData = types::InternalType::ScilabDouble;

    if (iRhs < 3 || iRhs > 5)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "read", 2, 4);
        return Function::Error;
    }

    //file
    if (in[0]->isString())
    {
        String* pSPath = in[0]->getAs<String>();

        if (pSPath->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A string expected.\n"), "read", 1);
            return Function::Error;
        }

        int piMode[2] = { -1, 0};
        char* pstFilename = wide_string_to_UTF8(pSPath->get(0));
        int iErr = C2F(clunit)(&iID, pstFilename, piMode, (int)strlen(pstFilename));

        if (iErr == 240)
        {
            Scierror(999, _("File \"%s\" already exists or directory write access denied.\n"), pstFilename);
            FREE(pstFilename);
            return Function::Error;
        }

        if (iErr == 241)
        {
            Scierror(999, _("File \"%s\" does not exist or read access denied.\n"), pstFilename);
            FREE(pstFilename);
            return Function::Error;
        }

        FREE(pstFilename);
    }
    else if (in[0]->isDouble())
    {
        Double* pDId = in[0]->getAs<Double>();
        if (pDId->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A real scalar expected.\n"), "read", 1);
            return Function::Error;
        }

        iID = (int)pDId->get(0);
        if (iID == -1)
        {
            iID = FileManager::getCurrentFile();
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A real scalar or file descriptor expected.\n"), "read", 1);
        return Function::Error;
    }

    if (iRhs > 3)
    {
        int iPos = iRhs - 1;
        if (in[iPos]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A string expected.\n"), "read", iRhs);
            return Function::Error;
        }

        String* pSFormat = in[iPos]->getAs<String>();
        if (pSFormat->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A string expected.\n"), "read", iRhs);
            return Function::Error;
        }

        //checkformat
        pstFormat = wide_string_to_UTF8(pSFormat->get(0));

        char cTypeData = checkformatread(pstFormat);
        if (cTypeData == 0)
        {
            Scierror(999, _("Incorrect file or format.\n"));
            return Function::Error;
        }
        switch (cTypeData)
        {
            case '1':
                itTypeOfData = types::InternalType::ScilabDouble;
                break;
            case '2':
                itTypeOfData = types::InternalType::ScilabInt32;
                break;
            case '3':
                itTypeOfData = types::InternalType::ScilabBool;
                break;
            case '4':
                itTypeOfData = types::InternalType::ScilabString;
                break;
            default:
                break;
        }

    }

    int error = 0;
    int iRows = (int) in[1]->getAs<Double>()->get(0);
    int iCols = (int) in[2]->getAs<Double>()->get(0);

    //test dims
    if ( (iCols <= 0) || (iRows == 0))
    {
        if (pstFormat != NULL)
        {
            FREE(pstFormat);
        }
        out.push_back(types::Double::Empty());
        return Function::OK;
    }
    if (iRows < 0)
    {
        if (iID != 5 /*stdout*/)
        {

            switch (itTypeOfData)
            {
                case types::InternalType::ScilabDouble:
                {
                    iRows = 1;
                    Double* pD = new types::Double(iRows, iCols, false);

                    if (pstFormat == NULL)
                    {
                        while (error == 0)
                        {
                            double* pdData = new double[iCols];
                            C2F(readdoublelinefile)(&iID, pdData, &iCols, &error);
                            if (error == 0)
                            {
                                pD->resize(iRows, iCols);
                                for (int i = 0; i < iCols; i++)
                                {
                                    pD->set((iRows - 1), i, pdData[i]);
                                }
                                iRows++;
                            }
                            delete[] pdData;

                        }

                    }
                    else
                    {
                        while (error == 0)
                        {
                            double* pdData = new double[iCols];
                            C2F(readdoublelinefileform)(&iID, pstFormat, pdData, &iCols, &error);
                            if (error == 0)
                            {
                                pD->resize(iRows, iCols);
                                for (int i = 0; i < iCols; i++)
                                {
                                    pD->set((iRows - 1), i, pdData[i]);
                                }
                                iRows++;
                            }
                            delete[] pdData;
                        }

                    }

                    if (error != 2)
                    {
                        out.push_back(pD);
                    }
                    else
                    {
                        delete pD;
                    }

                }
                break;
                case types::InternalType::ScilabInt32:
                {

                    iRows = 1;
                    types::Int32* pI = new types::Int32(iRows, iCols);

                    while (error == 0)
                    {
                        int* piData = new int[iCols];
                        C2F(readintlinefileform)(&iID, pstFormat, piData, &iCols, &error);
                        if (error == 0)
                        {
                            pI->resize(iRows, iCols);
                            for (int i = 0; i < iCols; i++)
                            {
                                pI->set((iRows - 1), i, piData[i]);
                            }
                            iRows++;
                        }
                        delete[] piData;

                    }

                    if (error != 2)
                    {
                        out.push_back(pI);
                    }
                    else
                    {
                        delete pI;
                    }
                }
                break;
                case types::InternalType::ScilabString:
                {

                    if (iCols != 1)
                    {
                        Scierror(999, _("%s: Wrong input argument %d.\n"), "read", 3);
                        return Function::Error;
                    }
                    else
                    {
                        bool bEndWrite = false;
                        iRows = 1;
                        String* pS = new types::String(iRows, iCols);

                        for (; bEndWrite == false; iRows++)
                        {
                            char* pCt = (char *)MALLOC(sizeof(char) * 4096);
                            int siz = 0;
                            C2F(readstringfile)(&iID, pstFormat, pCt, &siz, &error, (int)strlen(pstFormat));

                            if (error == 1)
                            {
                                bEndWrite = true;
                            }
                            else
                            {
                                char* pC = new char[(siz + 1)];
                                pC[0] = '\0';
                                strncat(pC, pCt, siz);
                                pS->resize(iRows, iCols);
                                pS->set((iRows - 1), (iCols - 1), pC);
                                delete[] pC;

                            }
                            FREE(pCt);

                        }

                        if (error != 2)
                        {
                            out.push_back(pS);
                        }
                        else
                        {
                            delete pS;
                        }
                    }

                }
                break;
                default:
                    Scierror(999, _("%s: Wrong type for input argument #%d : A string expected.\n"), "read", 2);

                    //close file
                    if (in[0]->isString())
                    {
                        int piMode[2] = { 0, 0 };
                        String* pSPath = in[0]->getAs<String>();
                        char* pstFilename = wide_string_to_UTF8(pSPath->get(0));
                        int  close = -iID;
                        int iErr = C2F(clunit)(&close, pstFilename, piMode, (int)strlen(pstFilename));
                        FREE(pstFilename);
                    }
                    return Function::Error;
                    break;
            }

            //close file
            if (in[0]->isString())
            {
                int piMode[2] = { 0, 0 };
                String* pSPath = in[0]->getAs<String>();
                char* pstFilename = wide_string_to_UTF8(pSPath->get(0));
                int  close = -iID;
                int iErr = C2F(clunit)(&close, pstFilename, piMode, (int)strlen(pstFilename));
                FREE(pstFilename);
            }
        }
        else//read from the console
        {

            switch (itTypeOfData)
            {
                case types::InternalType::ScilabDouble:
                {
                    iRows = 1;
                    Double* pD = new types::Double(iRows, iCols, false);

                    pstFormat = (char*)MALLOC(sizeof(char) * (strlen("(a)\0") + 1));
                    pstFormat[0] = '(';
                    pstFormat[1] = 'a';
                    pstFormat[2] = ')';
                    pstFormat[3] = '\0';

                    bool bEndWrite = false;

                    for (; (bEndWrite == false) && (error == 0); iRows++)
                    {
                        char* pCt = (char *)MALLOC(sizeof(char) * 4096);
                        int siz = 0;
                        C2F(readstring)(pstFormat, pCt, &siz, &error, (int)strlen(pstFormat));

                        if ((siz == 1) && (pCt[0] == ' '))
                        {
                            bEndWrite = true;
                        }
                        else
                        {
                            char* pC = (char *)MALLOC(sizeof(char) * (siz + 1));
                            pC[0] = '\0';
                            strncat(pC, pCt, siz);
                            FREE(pCt);

                            char* pch;
                            int iColsTempo = 0;
                            pch = strtok(pC, " ");
                            double* pdData = new double[iCols];
                            while ((pch != NULL) && (error == 0) && (iColsTempo < iCols))
                            {
                                if (is_of_type<double>(pch))
                                {

                                    pdData[iColsTempo] = atof(pch);
                                    iColsTempo++;
                                }
                                else
                                {
                                    error = 2;
                                }
                                pch = strtok(NULL, " ");
                            }

                            FREE(pC);

                            if (iColsTempo != iCols)
                            {
                                FREE(pstFormat);

                                delete[] pdData;
                                delete pD;

                                Scierror(999, _("End of file at line %d.\n"));
                                return Function::Error;
                            }
                            else
                            {
                                pD->resize(iRows, iCols);
                                for (int i = 0; i < iCols; i++)
                                {
                                    pD->set((iRows - 1), i, pdData[i]);
                                }
                            }
                        }

                    }

                    if (error == 0)
                    {
                        out.push_back(pD);
                    }
                }
                break;
                case types::InternalType::ScilabInt32:
                    Scierror(999, _("Incorrect file or format.\n"));
                    return Function::Error;
                case types::InternalType::ScilabString:
                {
                    if (iCols != 1)
                    {
                        Scierror(999, _("%s: Wrong input argument %d.\n"), "read", 3);
                        return Function::Error;
                    }
                    else
                    {
                        bool bEndWrite = false;
                        iRows = 1;
                        String* pS = new types::String(iRows, iCols);

                        for (; bEndWrite == false; iRows++)
                        {
                            char* pCt = (char *)MALLOC(sizeof(char) * 4096);
                            int siz = 0;
                            C2F(readstring)(pstFormat, pCt, &siz, &error, (int)strlen(pstFormat));
                            if ((siz == 1) && (pCt[0] == ' '))
                            {
                                bEndWrite = true;
                            }
                            else
                            {
                                char* pC = new char[(siz + 1)];
                                pC[0] = '\0';
                                strncat(pC, pCt, siz);
                                pS->resize(iRows, iCols);
                                pS->set((iRows - 1), (iCols - 1), pC);
                                delete[] pC;
                            }
                            FREE(pCt);

                        }

                        if (error == 0)
                        {
                            out.push_back(pS);
                        }
                        else
                        {
                            delete pS;
                        }
                    }
                }
                break;
                default:
                    break;
            }
        }
    }
    else
    {
        if (iID != 5 /*stdout*/)
        {

            switch (itTypeOfData)
            {
                case types::InternalType::ScilabDouble:
                {
                    Double* pD = new types::Double(iRows, iCols, false);
                    double* pd = pD->get();

                    if (pstFormat == NULL)
                    {
                        C2F(readdoublefile)(&iID, pd, &iRows, &iCols, &error);
                    }
                    else
                    {
                        C2F(readdoublefileform)(&iID, pstFormat, pd, &iRows, &iCols, &error, (int)strlen(pstFormat));
                    }

                    if (error == 0)
                    {
                        out.push_back(pD);
                    }
                    else
                    {
                        delete pD;
                    }
                }
                break;
                case types::InternalType::ScilabInt32:
                {
                    types::Int32* pI = new types::Int32(iRows, iCols);
                    int* pi = pI->get();

                    C2F(readintfileform)(&iID, pstFormat, pi, &iRows, &iCols, &error, (int)strlen(pstFormat));

                    if (error == 0)
                    {
                        out.push_back(pI);
                    }
                    else
                    {
                        delete pI;
                    }
                }
                break;
                case types::InternalType::ScilabString:
                {
                    String* pS = new types::String(iRows, iCols);
                    for (int i = 0; i < (iCols * iRows); i++)
                    {
                        char* pCt = (char *)MALLOC(sizeof(char) * 4096);
                        int siz = 0;
                        C2F(readstringfile)(&iID, pstFormat, pCt, &siz, &error, (int)strlen(pstFormat));
                        char* pC = (char *)MALLOC(sizeof(char) * (siz + 1));
                        pC[0] = '\0';
                        strncat(pC, pCt, siz);
                        pS->set(i, pC);
                        FREE(pCt);
                        FREE(pC);
                    }

                    if (error == 0)
                    {
                        out.push_back(pS);
                    }
                    else
                    {
                        delete pS;
                    }
                }
                break;
                default:
                    Scierror(999, _("%s: Wrong type for input argument #%d : A string expected.\n"), "read", 2);

                    //close file
                    if (in[0]->isString())
                    {
                        int piMode[2] = { 0, 0 };
                        String* pSPath = in[0]->getAs<String>();
                        char* pstFilename = wide_string_to_UTF8(pSPath->get(0));
                        int  close = -iID;
                        int iErr = C2F(clunit)(&close, pstFilename, piMode, (int)strlen(pstFilename));
                        FREE(pstFilename);
                    }
                    return Function::Error;
                    break;
            }

            //close file
            if (in[0]->isString())
            {
                int piMode[2] = { 0, 0 };
                String* pSPath = in[0]->getAs<String>();
                char* pstFilename = wide_string_to_UTF8(pSPath->get(0));
                int  close = -iID;
                int iErr = C2F(clunit)(&close, pstFilename, piMode, (int)strlen(pstFilename));
                FREE(pstFilename);
            }

            if (error == 2)
            {
                Scierror(999, _("Incorrect file or format.\n"));
                return Function::Error;
            }

        }
        else//read from the console
        {

            switch (itTypeOfData)
            {
                case types::InternalType::ScilabDouble:
                {
                    Double* pD = new types::Double(iRows, iCols, false);

                    pstFormat = (char*)MALLOC(sizeof(char) * (strlen("(a)\0") + 1));
                    pstFormat[0] = '(';
                    pstFormat[1] = 'a';
                    pstFormat[2] = ')';
                    pstFormat[3] = '\0';

                    for (int i = 0; i < (iRows) && (error == 0); i++)
                    {
                        char* pCt = (char *)MALLOC(sizeof(char) * 4096);
                        int siz = 0;
                        C2F(readstring)(pstFormat, pCt, &siz, &error, (int)strlen(pstFormat));
                        char* pC = (char *)MALLOC(sizeof(char) * (siz + 1));
                        strncat(pC, pCt, siz);
                        FREE(pCt);

                        char* pch;
                        int iColsTempo = 0;
                        pch = strtok(pC, " ");
                        while ((pch != NULL) && (error == 0))
                        {
                            if (is_of_type<double>(pch))
                            {
                                pD->set(i, iColsTempo, atof(pch));
                                iColsTempo++;
                            }
                            else
                            {
                                error = 2;
                            }
                            pch = strtok(NULL, " ");
                        }

                        FREE(pC);

                        if (iColsTempo != iCols)
                        {
                            delete pD;
                            FREE(pstFormat);

                            Scierror(999, _("End of file at line %d.\n"));
                            return Function::Error;
                        }
                    }

                    FREE(pstFormat);

                    if (error == 0)
                    {
                        out.push_back(pD);
                    }
                    else
                    {
                        delete pD;
                    }
                }
                break;
                case types::InternalType::ScilabInt32:
                    Scierror(999, _("Incorrect file or format.\n"));
                    return Function::Error;
                case types::InternalType::ScilabString:
                {
                    if (iCols != 1)
                    {
                        Scierror(999, _("%s: Wrong input argument %d.\n"), "read", 3);
                        return Function::Error;
                    }
                    else
                    {
                        String* pS = new types::String(iRows, iCols);

                        for (int i = 0; i < (iRows); i++)
                        {
                            char* pCt = (char *)MALLOC(sizeof(char) * 4096);
                            int siz = 0;
                            C2F(readstring)(pstFormat, pCt, &siz, &error, (int)strlen(pstFormat));
                            char* pC = (char *)MALLOC(sizeof(char) * (siz + 1));
                            strncat(pC, pCt, siz);
                            pS->set(i, pC);
                            FREE(pCt);
                            FREE(pC);
                        }


                        if (error == 0)
                        {
                            out.push_back(pS);
                        }
                        else
                        {
                            delete pS;
                        }
                    }
                }
                break;
                default:
                    break;
            }
        }

        if (error != 0)
        {
            Scierror(999, _("Incorrect file or format.\n"));
            return Function::Error;
        }

    }

    return Function::OK;
}

int checkformatread(char* format)
{
    char type1[] = { 'i', 'f', 'e', 'd', 'g', 'l', 'a', 'I', 'F', 'E', 'D', 'G', 'L', 'A' };
    char type2[] = { '2', '1', '1', '1', '1', '3', '4', '2', '1', '1', '1', '1', '3', '4' };
    int ret = 0;
    int size = (int)strlen(format);
    int count = 0;
    bool isString = false;
    if (size < 2 || format[0] != '(' || format[size - 1] != ')')
    {
        return 0;
    }

    for (int i = 1; i < size - 1; i++)
    {
        char c = format[i];

        if (c == '\'')
        {
            isString = !isString;
        }

        if (isString)
        {
            if (count == 0)
            {
                count++;
            }
            else
            {
                count--;
            }
            continue;
        }

        for (int j = 0; j < sizeof(type1); j++)
        {
            if (c == type1[j])
            {
                if (ret == 0)
                {
                    ret = type2[j];
                }

                if (type2[j] != ret)
                {
                    return 0;
                }

                break;
            }
        }
    }

    return ret;
}

template<typename T>
bool is_of_type(const std::string & Str)
{
    std::istringstream iss(Str);

    T tmp;
    return (iss >> tmp) && (iss.eof());
}
