/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
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

#include "types.hxx"
#include "double.hxx"
#include "string.hxx"
#include "scilab_sscanf.hxx"
#include "int.hxx"

extern "C"
{
#include <stdio.h>
#include "Scierror.h"
#include "sci_malloc.h"
#include "localization.h"
#include "charEncoding.h"
#include "BOOL.h"
#include "os_wcstok.h"
}

static wchar_t* findChars(wchar_t*, BOOL*); // Use to find what is inside the [] when the format is %[...].

int scilab_sscanf(wchar_t* _wcsFormat, wchar_t* _wcsData, int _iIterrator, int _iNiter, std::vector<types::InternalType*> *_pITOut)
{
    int i                       = 0;
    int j                       = 0;
    int nbrOfDigit              = 0;
    int dims                    = 2;
    int dimsArray[2]            = {_iNiter, 1};
    BOOL bStar                  = FALSE;
    BOOL bUnsigned              = FALSE;
    BOOL bIgnoredChars          = TRUE;
    int base                    = 0;
    wchar_t wcsLLH              = L' ';
    wchar_t* wcsData            = NULL;
    int sizeOfData              = (int)wcslen(_wcsData);
    int iCountDataRead          = 0;

    wcsData = (wchar_t*)MALLOC((sizeOfData + 1) * sizeof(wchar_t));
    memcpy(wcsData, _wcsData, sizeOfData * sizeof(wchar_t));
    wcsData[sizeOfData] = '\0';

    while (i < (int)wcslen(_wcsFormat))
    {
        while (bIgnoredChars && i < (int)wcslen(_wcsFormat)) // %da%s => 'a' is an ignored char.
        {
            if (wcsData != NULL && wcsData[0] != L'\0' && _wcsFormat[i] == wcsData[0])
            {
                if (_wcsFormat[i] != L' ')
                {
                    i++;
                    wcsData++;
                }
                else
                {
                    while (wcsData[0] == L' ')
                    {
                        wcsData++;
                    }
                    while (_wcsFormat[i] == L' ')
                    {
                        i++;
                    }
                }
            }
            else if ((wcsData == NULL || wcsData[0] == L'\0') && i < (int)wcslen(_wcsFormat) && iCountDataRead == 0 && _pITOut->size() == 0)
            {
                iCountDataRead = -1;
            }
            else
            {
                if (_wcsFormat[i] == L' ')
                {
                    do
                    {
                        i++;
                    }
                    while (i < (int)wcslen(_wcsFormat) && _wcsFormat[i] == L' ');
                }

                if (_wcsFormat[i] != L'%')
                {
                    FREE(wcsData);
                    wcsData = NULL;
                    while (i < (int)wcslen(_wcsFormat) && _wcsFormat[i] != L'%')
                    {
                        i++;
                    }
                }
                break;
            }
        }
        if (i == (int)wcslen(_wcsFormat))
        {
            break;
        }

        if (iswdigit(_wcsFormat[i]))
        {
            nbrOfDigit = wcstol(&_wcsFormat[i], NULL, 10);
            while (iswdigit(_wcsFormat[i]))
            {
                i++;
            }
        }
        else switch (_wcsFormat[i])
            {
                case L' ' :
                case L'\n':
                case L'\t':
                    i++;
                    break;
                case L'%' :
                    i++;
                    bIgnoredChars = FALSE;
                    break;
                case L'*' :
                    bStar = TRUE;
                    i++;
                    break;
                case L'h' :
                case L'l' :
                case L'L' :
                    wcsLLH = _wcsFormat[i];
                    i++;
                    break;
                case L'c' :
                {
                    if (wcsData != NULL && wcsData[0] != L'\0') // If the end of data has not been reached we can get datas.
                    {
                        wchar_t wcSingleData[2];
                        wcSingleData[0] = wcsData[0];
                        wcSingleData[1] = 0;

                        if (!bStar) // If this format is not ignored put the datas found.
                        {
                            if (_iIterrator == 0) // Create and initialize the container only the first time.
                            {
                                types::String* pS = new types::String(dims, dimsArray);
                                for (int k = 0 ; k < pS->getSize(); k++)
                                {
                                    pS->set(k, L"");
                                }
                                _pITOut->push_back(pS);
                            }
                            (*_pITOut)[j]->getAs<types::String>()->set(_iIterrator, wcSingleData);
                            iCountDataRead++;
                        }
                        wcsData++;
                    }
                    else
                    {
                        if (_iIterrator == 0 && !bStar && _iNiter == 1)
                        {
                            _pITOut->push_back(types::Double::Empty());
                        }
                        else
                        {
                            FREE(wcsData);
                            return -10;
                        }
                    }
                    if (!bStar)
                    {
                        j++;
                    }
                    i++;
                    bIgnoredChars = TRUE;
                    bStar = FALSE;
                }
                break;
                case L's' :
                {
                    if (wcsData != NULL && wcsData[0] != L'\0')
                    {
                        wchar_t* wcsSingleData  = NULL;
                        wchar_t* wcsRes         = NULL;
                        wchar_t seps[]          = L" \t\n";
                        int sizeOfCurrentData   = (int)wcslen(wcsData);
                        wchar_t* wcsTemp        = (wchar_t*)MALLOC((sizeOfCurrentData + 1) * sizeof(wchar_t));

                        memcpy(wcsTemp, wcsData, sizeOfCurrentData * sizeof(wchar_t));
                        wcsTemp[sizeOfCurrentData] = L'\0';
                        wcsRes = os_wcstok(wcsTemp, seps, &wcsTemp); // the seps[] found is replaced by the '\0' char.

                        if (wcsTemp == NULL || wcsTemp[0] == L'\0')
                        {
                            FREE(wcsData);
                            wcsData = NULL;
                        }
                        else
                        {
                            wcsData += (wcslen(wcsData) - wcslen(wcsTemp) - 1); // set the pointer on the seps[] and not on the next char.
                        }

                        if (nbrOfDigit) // Get only the numbers of digit indicated in the format. (ex: %2d)
                        {
                            wcsSingleData = (wchar_t*)MALLOC(sizeof(wchar_t) * (nbrOfDigit + 1));
                            memcpy(wcsSingleData, wcsRes, sizeof(wchar_t) * nbrOfDigit);
                            wcsSingleData[nbrOfDigit] = L'\0';
                            nbrOfDigit = 0;
                        }
                        else // Get all data find.
                        {
                            wcsSingleData = wcsRes;
                        }

                        if (!bStar)
                        {
                            if (_iIterrator == 0)
                            {
                                types::String* pS = new types::String(dims, dimsArray);
                                for (int k = 0 ; k < pS->getSize(); k++)
                                {
                                    pS->set(k, L"");
                                }
                                _pITOut->push_back(pS);
                            }
                            (*_pITOut)[j]->getAs<types::String>()->set(_iIterrator, wcsSingleData);
                            iCountDataRead++;
                        }

                        if (nbrOfDigit)
                        {
                            FREE(wcsSingleData);
                        }
                    }
                    else
                    {
                        if (_iIterrator == 0 && !bStar && _iNiter == 1)
                        {
                            _pITOut->push_back(types::Double::Empty());
                        }
                        else
                        {
                            FREE(wcsData);
                            return -10;
                        }
                    }
                    if (!bStar)
                    {
                        j++;
                    }
                    i++;
                    bIgnoredChars = TRUE;
                    bStar = FALSE;
                }
                break;
                case L'[' :
                {
                    if (wcsData != NULL && wcsData[0] != L'\0')
                    {
                        wchar_t* wcsInside          = NULL;
                        wchar_t* wcsCpyFormat       = NULL;
                        unsigned int iPos           = 0;
                        wchar_t* wcsSingleData      = NULL;
                        wchar_t* wcsRes             = NULL;
                        wchar_t* wscToFind          = NULL;
                        BOOL bInv                   = FALSE;

                        i++;
                        wcsCpyFormat = (wchar_t*)MALLOC((wcslen(_wcsFormat) - i + 1) * sizeof(wchar_t));
                        memcpy(wcsCpyFormat, &_wcsFormat[i], (wcslen(_wcsFormat) - i) * sizeof(wchar_t));
                        wcsCpyFormat[wcslen(_wcsFormat) - i] = L'\0';

                        wcsInside = os_wcstok(wcsCpyFormat, L"]", &wcsCpyFormat);
                        i += (int)wcslen(wcsInside) + 1; // +1 => ]

                        wscToFind = findChars(wcsInside, &bInv);
                        if (wscToFind == NULL)
                        {
                            // MALLOC error
                            FREE(wcsData);
                            return -10;
                        }

                        if (bInv)
                        {
                            iPos = (int)wcscspn(wcsData, wscToFind);
                        }
                        else
                        {
                            iPos = (int)wcsspn(wcsData, wscToFind);
                        }
                        FREE(wscToFind);
                        if (iPos == 0)
                        {
                            // The string begins with a character which is not in wscToFind
                            if (_iIterrator == 0 && !bStar && _iNiter == 1)
                            {
                                _pITOut->push_back(types::Double::Empty());
                            }
                            else
                            {
                                FREE(wcsData);
                                return -10;
                            }
                        }
                        else
                        {
                            wcsRes = (wchar_t*)MALLOC((iPos + 1) * sizeof(wchar_t));
                            memcpy(wcsRes, wcsData, iPos * sizeof(wchar_t));
                            wcsRes[iPos] = '\0';

                            FREE(wcsInside);

                            if (nbrOfDigit)
                            {
                                wcsSingleData = (wchar_t*)MALLOC(sizeof(wchar_t) * (nbrOfDigit + 1));
                                memcpy(wcsSingleData, wcsRes, sizeof(wchar_t) * nbrOfDigit);
                                wcsSingleData[nbrOfDigit] = L'\0';
                                wcsData += nbrOfDigit;
                            }
                            else
                            {
                                wcsSingleData = wcsRes;
                                wcsData += iPos;
                            }
                        }

                        if (!bStar)
                        {
                            if (_iIterrator == 0)
                            {
                                types::String* pS = new types::String(dims, dimsArray);
                                for (int k = 0 ; k < pS->getSize(); k++)
                                {
                                    pS->set(k, L"");
                                }
                                _pITOut->push_back(pS);
                            }
                            if (wcsSingleData != NULL)
                            {
                                (*_pITOut)[j]->getAs<types::String>()->set(_iIterrator, wcsSingleData);
                                iCountDataRead++;
                            }
                        }

                        if (nbrOfDigit)
                        {
                            FREE(wcsSingleData);
                        }
                    }
                    else
                    {
                        if (_iIterrator == 0 && !bStar && _iNiter == 1)
                        {
                            _pITOut->push_back(types::Double::Empty());
                        }
                        else
                        {
                            FREE(wcsData);
                            return -10;
                        }
                    }
                    if (!bStar)
                    {
                        j++;
                    }
                    bIgnoredChars   = TRUE;
                    nbrOfDigit      = 0;
                    bStar           = FALSE;
                }
                break;
                case L'x' :
                case L'X' :
                    base += 6; // 6 + 2 + 8 = 16 // Compute the base of data to get.
                case L'u' :
                    if (base == 0)
                    {
                        bUnsigned = TRUE;    // unsigned int
                    }
                case L'i' :
                case L'd' :
                    base += 2; // 2 + 8 = 10
                case L'o' :
                    base += 8; // 8 = 8 :D
                    {
                        if (wcsData != NULL && wcsData[0] != L'\0')
                        {
                            long int iSingleData = 0;
                            while (wcsData[0] == L' ')
                            {
                                wcsData++;
                            }
                            if (nbrOfDigit)
                            {
                                wchar_t* number = NULL;
                                if (wcslen(wcsData) < nbrOfDigit)
                                {
                                    nbrOfDigit = (int)wcslen(wcsData);
                                }

                                number = (wchar_t*)MALLOC((nbrOfDigit + 1) * sizeof(wchar_t));
                                memcpy(number, wcsData, nbrOfDigit * sizeof(wchar_t));
                                number[nbrOfDigit] = L'\0';
                                iSingleData = wcstoul(number, &number, base);
                                if ((iSingleData == 0) && (number[0] == wcsData[0]))
                                {
                                    if (_iIterrator == 0 && !bStar && _iNiter == 1)
                                    {
                                        FREE(wcsData);
                                        wcsData = NULL;
                                        _pITOut->push_back(types::Double::Empty());
                                        bStar = TRUE;
                                    }
                                    else
                                    {
                                        FREE(wcsData);
                                        return -10;
                                    }
                                }
                                if (number == NULL)
                                {
                                    wcsData += nbrOfDigit;
                                }
                                else
                                {
                                    wcsData += (nbrOfDigit - wcslen(number));
                                }
                                nbrOfDigit = 0;
                            }
                            else
                            {
                                wchar_t temp = wcsData[0];
                                iSingleData = wcstoul(wcsData, &wcsData, base);
                                if ((iSingleData == 0) && (temp == wcsData[0]))
                                {
                                    FREE(wcsData);
                                    wcsData = NULL;
                                    if (_iIterrator == 0 && !bStar && _iNiter == 1)
                                    {
                                        _pITOut->push_back(types::Double::Empty());
                                        bStar = TRUE;
                                    }
                                    else
                                    {
                                        return -10;
                                    }
                                }
                            }

                            if (!bStar)
                            {
                                if (_iIterrator == 0)
                                {
                                    switch (wcsLLH)
                                    {
                                        case L'h' :
                                        {
                                            if (bUnsigned)
                                            {
                                                types::UInt16* pUInt16 = new types::UInt16(dims, dimsArray);
                                                for (int k = 0; k < pUInt16->getSize(); k++)
                                                {
                                                    pUInt16->set(k, 0);
                                                }
                                                _pITOut->push_back(pUInt16);
                                            }
                                            else
                                            {
                                                types::Int16* pInt16 = new types::Int16(dims, dimsArray);
                                                for (int k = 0; k < pInt16->getSize(); k++)
                                                {
                                                    pInt16->set(k, 0);
                                                }
                                                _pITOut->push_back(pInt16);
                                            }
                                        }
                                        break;
                                        case L'l' :
                                        case L'L' :
                                        {
                                            if (bUnsigned)
                                            {
                                                types::UInt64* pUInt64 = new types::UInt64(dims, dimsArray);
                                                for (int k = 0; k < pUInt64->getSize(); k++)
                                                {
                                                    pUInt64->set(k, 0);
                                                }
                                                _pITOut->push_back(pUInt64);
                                            }
                                            else
                                            {
                                                types::Int64* pInt64 = new types::Int64(dims, dimsArray);
                                                for (int k = 0; k < pInt64->getSize(); k++)
                                                {
                                                    pInt64->set(k, 0);
                                                }
                                                _pITOut->push_back(pInt64);
                                            }
                                        }
                                        break;
                                        default :
                                        {
                                            if (bUnsigned)
                                            {
                                                types::UInt32* pUInt32 = new types::UInt32(dims, dimsArray);
                                                for (int k = 0; k < pUInt32->getSize(); k++)
                                                {
                                                    pUInt32->set(k, 0);
                                                }
                                                _pITOut->push_back(pUInt32);
                                            }
                                            else
                                            {
                                                types::Int32* pInt32 = new types::Int32(dims, dimsArray);
                                                for (int k = 0; k < pInt32->getSize(); k++)
                                                {
                                                    pInt32->set(k, 0);
                                                }
                                                _pITOut->push_back(pInt32);
                                            }
                                        }
                                    }
                                }
                                switch (wcsLLH)
                                {
                                    case L'h' :
                                        if (bUnsigned)
                                        {
                                            (*_pITOut)[j]->getAs<types::UInt16>()->set(_iIterrator, static_cast<unsigned short int>(iSingleData));
                                            iCountDataRead++;
                                        }
                                        else
                                        {
                                            (*_pITOut)[j]->getAs<types::Int16>()->set(_iIterrator, static_cast<short int>(iSingleData));
                                            iCountDataRead++;
                                        }
                                        break;
                                    case L'l' :
                                    case L'L' :
                                        if (bUnsigned)
                                        {
                                            (*_pITOut)[j]->getAs<types::UInt64>()->set(_iIterrator, iSingleData);
                                            iCountDataRead++;
                                        }
                                        else
                                        {
                                            (*_pITOut)[j]->getAs<types::Int64>()->set(_iIterrator, static_cast<long int>(iSingleData));
                                            iCountDataRead++;
                                        }
                                        break;
                                    default :
                                        if (bUnsigned)
                                        {
                                            (*_pITOut)[j]->getAs<types::UInt32>()->set(_iIterrator, static_cast<unsigned int>(iSingleData));
                                            iCountDataRead++;
                                        }
                                        else
                                        {
                                            (*_pITOut)[j]->getAs<types::Int32>()->set(_iIterrator, static_cast<int>(iSingleData));
                                            iCountDataRead++;
                                        }
                                }
                            }
                        }
                        else
                        {
                            if (_iIterrator == 0 && !bStar && _iNiter == 1)
                            {
                                _pITOut->push_back(types::Double::Empty());
                            }
                            else
                            {
                                FREE(wcsData);
                                return -10;
                            }
                        }
                        if (!bStar)
                        {
                            j++;
                        }
                        wcsLLH          = L' ';
                        bIgnoredChars   = TRUE;
                        bUnsigned       = FALSE;
                        bStar           = FALSE;
                        base            = 0;
                        i++;
                    }
                    break;
                case L'e' :
                case L'E' :
                case L'g' :
                case L'G' :
                case L'f' :
                {
                    if (wcsData != NULL && wcsData[0] != L'\0')
                    {
                        double dSingleData  = 0;
                        BOOL bSigne         = FALSE;
                        while (wcsData[0] == L' ')
                        {
                            wcsData++;
                        }
                        if (nbrOfDigit)
                        {
                            int iSizeRead   = 0;
                            wchar_t* number = NULL;
                            wchar_t* next   = NULL;
                            if (wcslen(wcsData) < nbrOfDigit)
                            {
                                nbrOfDigit = (int)wcslen(wcsData);
                            }
                            number = (wchar_t*)MALLOC((nbrOfDigit + 1) * sizeof(wchar_t));
                            memcpy(number, wcsData, nbrOfDigit * sizeof(wchar_t));
                            number[nbrOfDigit] = L'\0';
                            dSingleData = wcstod(number, &next);
                            if (next)
                            {
                                iSizeRead = nbrOfDigit - (int)wcslen(next);
                            }
                            else
                            {
                                iSizeRead = nbrOfDigit;
                            }
                            if ((dSingleData == 0) && (number[0] == next[0]))
                            {
                                if (_iIterrator == 0 && !bStar && _iNiter == 1)
                                {
                                    wcsData = NULL;
                                    _pITOut->push_back(types::Double::Empty());
                                    bStar = TRUE;
                                }
                                else
                                {
                                    FREE(wcsData);
                                    return -10;
                                }
                            }
                            wcsData += iSizeRead;
                            FREE(number);
                            nbrOfDigit = 0;
                        }
                        else
                        {
                            int iLenData = (int)wcslen(wcsData);
                            dSingleData = wcstod(wcsData, &wcsData);
                            if ((dSingleData == 0) && (iLenData == wcslen(wcsData)))
                            {
                                if (_iIterrator == 0 && !bStar && _iNiter == 1)
                                {
                                    FREE(wcsData);
                                    wcsData = NULL;
                                    _pITOut->push_back(types::Double::Empty());
                                    bStar = TRUE;
                                }
                                else
                                {
                                    FREE(wcsData);
                                    return -10;
                                }
                            }
                        }

                        if (!bStar)
                        {
                            if (_iIterrator == 0)
                            {
                                types::Double* pD = new types::Double(dims, dimsArray);
                                for (int k = 0 ; k < pD->getSize(); k++)
                                {
                                    pD->set(k, 0);
                                }
                                _pITOut->push_back(pD);
                            }
                            (*_pITOut)[j]->getAs<types::Double>()->set(_iIterrator, dSingleData);
                            iCountDataRead++;
                        }
                    }
                    else
                    {
                        if (_iIterrator == 0 && !bStar && _iNiter == 1)
                        {
                            _pITOut->push_back(types::Double::Empty());
                        }
                        else
                        {
                            FREE(wcsData);
                            return -10;
                        }
                    }
                    if (!bStar)
                    {
                        j++;
                    }
                    i++;
                    bIgnoredChars = TRUE;
                    bStar = FALSE;
                }
                break;
                case L'n' :
                {
                    double dSingleData = -1;
                    if (_iIterrator == 0 && !bStar)
                    {
                        types::Double* pD = new types::Double(dims, dimsArray);
                        for (int k = 0 ; k < pD->getSize(); k++)
                        {
                            pD->set(k, 0);
                        }
                        _pITOut->push_back(pD);
                    }

                    if (wcsData == NULL || wcsData[0] == L'\0')
                    {
                        dSingleData = (double)sizeOfData;
                    }
                    else
                    {
                        dSingleData = (double)sizeOfData - (double)wcslen(wcsData);
                    }

                    if (!bStar)
                    {
                        (*_pITOut)[j]->getAs<types::Double>()->set(_iIterrator, dSingleData);
                        j++;
                    }

                    i++;
                    bIgnoredChars = TRUE;
                    bStar = FALSE;
                }
                break;
                default :
                    printf("format read : %c\n", _wcsFormat[i]);
                    FREE(wcsData);
                    return -10;
            }
    }
    FREE(wcsData);
    return iCountDataRead;
}

wchar_t* findChars(wchar_t* chars, BOOL* _bInv) // ex: "123456789abc" = findChars("1-9abc")
{
    unsigned int iIterChars = 0;
    unsigned int iIterRes   = 0;
    int iNbr                = 0;
    int iLen                = 0;
    wchar_t* wcsRes         = NULL;
    int* piPos              = NULL;

    piPos = (int*)MALLOC(wcslen(chars) * sizeof(int));

    if (chars[0] == L'^')
    {
        *_bInv = TRUE;
    }
    else
    {
        *_bInv = FALSE;
    }

    for (iIterChars = 0; iIterChars < wcslen(chars); iIterChars++)
    {
        if (chars[iIterChars] == L'-' && iIterChars != 0 && iIterChars != wcslen(chars) - 1)
        {
            int iSize = chars[iIterChars + 1] - chars[iIterChars - 1] - 1;
            if (iSize >= 0)
            {
                iLen += iSize;
                iNbr++;
                piPos[iIterChars] = 1;
            }
            else
            {
                piPos[iIterChars] = 0;
                iLen++;
            }
        }
        else
        {
            piPos[iIterChars] = 0;
            iLen++;
        }
    }

    wcsRes = (wchar_t*)MALLOC((iLen + 1) * sizeof(wchar_t));

    for (iIterChars = 0; iIterChars < wcslen(chars); iIterChars++)
    {
        if (piPos[iIterChars])
        {
            for (int i = 1; i < (chars[iIterChars + 1] - chars[iIterChars - 1]); i++, iIterRes++)
            {
                wcsRes[iIterRes] = chars[iIterChars - 1] + i;
            }
        }
        else
        {
            wcsRes[iIterRes] = chars[iIterChars];
            iIterRes++;
        }
    }
    wcsRes[iLen] = L'\0';

    FREE(piPos);
    return wcsRes;
}
