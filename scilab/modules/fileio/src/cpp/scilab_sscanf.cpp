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

static char* findChars(const char*, BOOL*); // Use to find what is inside the [] when the format is %[...].

int scilab_sscanf(const char* _format, const char* _data, int _iIterrator, int _iNiter, std::vector<types::InternalType*> *_pITOut)
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
    char LLH                    = ' ';
    char* data                  = NULL;
    int sizeOfData              = (int)strlen(_data);
    int iCountDataRead          = 0;

    data = (char*)MALLOC((sizeOfData + 1) * sizeof(char));
    memcpy(data, _data, sizeOfData * sizeof(char));
    data[sizeOfData] = '\0';

    while (i < (int)strlen(_format))
    {
        while (bIgnoredChars && i < (int)strlen(_format)) // %da%s => 'a' is an ignored char.
        {
            if (data != NULL && data[0] != '\0' && _format[i] == data[0])
            {
                if (_format[i] != ' ')
                {
                    i++;
                    data++;
                }
                else
                {
                    while (data[0] == ' ')
                    {
                        data++;
                    }
                    while (_format[i] == ' ')
                    {
                        i++;
                    }
                }
            }
            else if ((data == NULL || data[0] == '\0') && i < (int)strlen(_format) && iCountDataRead == 0 && _pITOut->size() == 0)
            {
                iCountDataRead = -1;
            }
            else
            {
                if (_format[i] == ' ')
                {
                    do
                    {
                        i++;
                    }
                    while (i < (int)strlen(_format) && _format[i] == ' ');
                }

                if (_format[i] != '%')
                {
                    data = NULL;
                    while (i < (int)strlen(_format) && _format[i] != '%')
                    {
                        i++;
                    }
                }
                break;
            }
        }
        if (i == (int)strlen(_format))
        {
            break;
        }

        if (iswdigit(_format[i]))
        {
            nbrOfDigit = strtol(&_format[i], NULL, 10);
            while (isdigit(_format[i]))
            {
                i++;
            }
        }
        else switch (_format[i])
            {
                case ' ' :
                case '\n':
                case '\t':
                    i++;
                    break;
                case '%' :
                    i++;
                    bIgnoredChars = FALSE;
                    break;
                case '*' :
                    bStar = TRUE;
                    i++;
                    break;
                case 'h' :
                case 'l' :
                case 'L' :
                    LLH = _format[i];
                    i++;
                    break;
                case 'c' :
                {
                    if (data != NULL && data[0] != '\0') // If the end of data has not been reached we can get datas.
                    {
                        char wcSingleData[2];
                        wcSingleData[0] = data[0];
                        wcSingleData[1] = 0;

                        if (!bStar) // If this format is not ignored put the datas found.
                        {
                            if (_iIterrator == 0) // Create and initialize the container only the first time.
                            {
                                types::String* pS = new types::String(dims, dimsArray);
                                for (int k = 0 ; k < pS->getSize(); k++)
                                {
                                    pS->set(k, "");
                                }
                                _pITOut->push_back(pS);
                            }
                            (*_pITOut)[j]->getAs<types::String>()->set(_iIterrator, wcSingleData);
                            iCountDataRead++;
                        }
                        data++;
                    }
                    else
                    {
                        if (_iIterrator == 0 && !bStar && _iNiter == 1)
                        {
                            _pITOut->push_back(types::Double::Empty());
                        }
                        else
                        {
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
                case 's' :
                {
                    if (data != NULL && data[0] != '\0')
                    {
                        char* singleData        = NULL;
                        char* res               = NULL;
                        char seps[]             = " \t\n";
                        int sizeOfCurrentData   = (int)strlen(data);
                        char* temp              = (char*)MALLOC((sizeOfCurrentData + 1) * sizeof(char));

                        memcpy(temp, data, sizeOfCurrentData * sizeof(char));
                        temp[sizeOfCurrentData] = '\0';
                        res = os_strtok(temp, seps, &temp); // the seps[] found is replaced by the '\0' char.

                        if (temp == NULL || temp[0] == '\0')
                        {
                            data = NULL;
                        }
                        else
                        {
                            data += (strlen(data) - strlen(temp) - 1); // set the pointer on the seps[] and not on the next char.
                        }

                        if (nbrOfDigit) // Get only the numbers of digit indicated in the format. (ex: %2d)
                        {
                            singleData = (char*)MALLOC(sizeof(char) * (nbrOfDigit + 1));
                            memcpy(singleData, res, sizeof(char) * nbrOfDigit);
                            singleData[nbrOfDigit] = '\0';
                            nbrOfDigit = 0;
                        }
                        else // Get all data find.
                        {
                            singleData = res;
                        }

                        if (!bStar)
                        {
                            if (_iIterrator == 0)
                            {
                                types::String* pS = new types::String(dims, dimsArray);
                                for (int k = 0 ; k < pS->getSize(); k++)
                                {
                                    pS->set(k, "");
                                }
                                _pITOut->push_back(pS);
                            }
                            (*_pITOut)[j]->getAs<types::String>()->set(_iIterrator, singleData);
                            iCountDataRead++;
                        }

                        if (nbrOfDigit)
                        {
                            FREE(singleData);
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
                case '[' :
                {
                    if (data != NULL && data[0] != '\0')
                    {
                        char* inside        = NULL;
                        char* cpyFormat     = NULL;
                        unsigned int iPos   = 0;
                        char* singleData    = NULL;
                        char* res           = NULL;
                        char* toFind        = NULL;
                        BOOL bInv           = FALSE;

                        i++;
                        cpyFormat = (char*)MALLOC((strlen(_format) - i + 1) * sizeof(char));
                        memcpy(cpyFormat, &_format[i], (strlen(_format) - i) * sizeof(char));
                        cpyFormat[strlen(_format) - i] = '\0';

                        inside = os_strtok(cpyFormat, "]", &cpyFormat);
                        i += (int)strlen(inside) + 1; // +1 => ]

                        toFind = findChars(inside, &bInv);
                        if (toFind == NULL)
                        {
                            // MALLOC error
                            return -10;
                        }

                        if (bInv)
                        {
                            iPos = (int)strcspn(data, toFind);
                        }
                        else
                        {
                            iPos = (int)strspn(data, toFind);
                        }

                        FREE(toFind);

                        if (iPos == 0)
                        {
                            // The string begins with a character which is not in toFind
                            if (_iIterrator == 0 && !bStar && _iNiter == 1)
                            {
                                _pITOut->push_back(types::Double::Empty());
                            }
                            else
                            {
                                return -10;
                            }
                        }
                        else
                        {
                            res = (char*)MALLOC((iPos + 1) * sizeof(char));
                            memcpy(res, data, iPos * sizeof(char));
                            res[iPos] = '\0';

                            FREE(inside);

                            if (nbrOfDigit)
                            {
                                singleData = (char*)MALLOC(sizeof(char) * (nbrOfDigit + 1));
                                memcpy(singleData, res, sizeof(char) * nbrOfDigit);
                                singleData[nbrOfDigit] = '\0';
                                data += nbrOfDigit;
                            }
                            else
                            {
                                singleData = res;
                                data += iPos;
                            }
                        }

                        if (!bStar)
                        {
                            if (_iIterrator == 0)
                            {
                                types::String* pS = new types::String(dims, dimsArray);
                                for (int k = 0 ; k < pS->getSize(); k++)
                                {
                                    pS->set(k, "");
                                }
                                _pITOut->push_back(pS);
                            }
                            if (singleData != NULL)
                            {
                                (*_pITOut)[j]->getAs<types::String>()->set(_iIterrator, singleData);
                                iCountDataRead++;
                            }
                        }

                        if (nbrOfDigit)
                        {
                            FREE(singleData);
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
                case 'x' :
                case 'X' :
                    base += 6; // 6 + 2 + 8 = 16 // Compute the base of data to get.
                case 'u' :
                    if (base == 0)
                    {
                        bUnsigned = TRUE;    // unsigned int
                    }
                case 'i' :
                case 'd' :
                    base += 2; // 2 + 8 = 10
                case 'o' :
                    base += 8; // 8 = 8 :D
                    {
                        if (data != NULL && data[0] != '\0')
                        {
                            long int iSingleData = 0;
                            while (data[0] == ' ')
                            {
                                data++;
                            }
                            if (nbrOfDigit)
                            {
                                char* number = NULL;
                                if (strlen(data) < nbrOfDigit)
                                {
                                    nbrOfDigit = (int)strlen(data);
                                }

                                number = (char*)MALLOC((nbrOfDigit + 1) * sizeof(char));
                                memcpy(number, data, nbrOfDigit * sizeof(char));
                                number[nbrOfDigit] = '\0';
                                iSingleData = strtoul(number, &number, base);
                                if ((iSingleData == 0) && (number[0] == data[0]))
                                {
                                    if (_iIterrator == 0 && !bStar && _iNiter == 1)
                                    {
                                        data = NULL;
                                        _pITOut->push_back(types::Double::Empty());
                                        bStar = TRUE;
                                    }
                                    else
                                    {
                                        return -10;
                                    }
                                }
                                if (number == NULL)
                                {
                                    data += nbrOfDigit;
                                }
                                else
                                {
                                    data += (nbrOfDigit - strlen(number));
                                }
                                nbrOfDigit = 0;
                            }
                            else
                            {
                                char temp = data[0];
                                iSingleData = strtoul(data, &data, base);
                                if ((iSingleData == 0) && (temp == data[0]))
                                {
                                    data = NULL;
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
                                    switch (LLH)
                                    {
                                        case 'h' :
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
                                        case 'l' :
                                        case 'L' :
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
                                switch (LLH)
                                {
                                    case 'h' :
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
                                    case 'l' :
                                    case 'L' :
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
                                return -10;
                            }
                        }
                        if (!bStar)
                        {
                            j++;
                        }
                        LLH          = ' ';
                        bIgnoredChars   = TRUE;
                        bUnsigned       = FALSE;
                        bStar           = FALSE;
                        base            = 0;
                        i++;
                    }
                    break;
                case 'e' :
                case 'E' :
                case 'g' :
                case 'G' :
                case 'f' :
                {
                    if (data != NULL && data[0] != '\0')
                    {
                        double dSingleData  = 0;
                        BOOL bSigne         = FALSE;
                        while (data[0] == ' ')
                        {
                            data++;
                        }
                        if (nbrOfDigit)
                        {
                            int iSizeRead   = 0;
                            char* number = NULL;
                            char* next   = NULL;
                            if (strlen(data) < nbrOfDigit)
                            {
                                nbrOfDigit = (int)strlen(data);
                            }
                            number = (char*)MALLOC((nbrOfDigit + 1) * sizeof(char));
                            memcpy(number, data, nbrOfDigit * sizeof(char));
                            number[nbrOfDigit] = '\0';
                            dSingleData = strtod(number, &next);
                            if (next)
                            {
                                iSizeRead = nbrOfDigit - (int)strlen(next);
                            }
                            else
                            {
                                iSizeRead = nbrOfDigit;
                            }
                            if ((dSingleData == 0) && (number[0] == next[0]))
                            {
                                if (_iIterrator == 0 && !bStar && _iNiter == 1)
                                {
                                    data = NULL;
                                    _pITOut->push_back(types::Double::Empty());
                                    bStar = TRUE;
                                }
                                else
                                {
                                    return -10;
                                }
                            }
                            data += iSizeRead;
                            FREE(number);
                            nbrOfDigit = 0;
                        }
                        else
                        {
                            int iLenData = (int)strlen(data);
                            dSingleData = strtod(data, &data);
                            if ((dSingleData == 0) && (iLenData == strlen(data)))
                            {
                                data = NULL;
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
                case 'n' :
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

                    if (data == NULL || data[0] == '\0')
                    {
                        dSingleData = (double)sizeOfData;
                    }
                    else
                    {
                        dSingleData = (double)sizeOfData - (double)strlen(data);
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
                    printf("format read : %c\n", _format[i]);
                    return -10;
            }
    }

    return iCountDataRead;
}

char* findChars(const char* chars, BOOL* _bInv) // ex: "123456789abc" = findChars("1-9abc")
{
    unsigned int iIterChars = 0;
    unsigned int iIterRes   = 0;
    int iNbr                = 0;
    int iLen                = 0;
    char* res               = NULL;
    int* piPos              = NULL;

    piPos = (int*)MALLOC(strlen(chars) * sizeof(int));

    if (chars[0] == '^')
    {
        *_bInv = TRUE;
    }
    else
    {
        *_bInv = FALSE;
    }

    for (iIterChars = 0; iIterChars < strlen(chars); iIterChars++)
    {
        if (chars[iIterChars] == '-' && iIterChars != 0 && iIterChars != strlen(chars) - 1)
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

    res = (char*)MALLOC((iLen + 1) * sizeof(char));

    for (iIterChars = 0; iIterChars < strlen(chars); iIterChars++)
    {
        if (piPos[iIterChars])
        {
            for (int i = 1; i < (chars[iIterChars + 1] - chars[iIterChars - 1]); i++, iIterRes++)
            {
                res[iIterRes] = chars[iIterChars - 1] + i;
            }
        }
        else
        {
            res[iIterRes] = chars[iIterChars];
            iIterRes++;
        }
    }
    res[iLen] = '\0';

    FREE(piPos);
    return res;
}
