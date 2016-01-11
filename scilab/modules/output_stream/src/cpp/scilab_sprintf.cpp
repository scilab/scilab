/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
 *  Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
 *  Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <stdio.h>
#include <cmath>
#include <list>
#include "types.hxx"
#include "double.hxx"
#include "string.hxx"
#include "scilab_sprintf.hxx"

extern "C"
{
#include "Scierror.h"
#include "sci_malloc.h"
#include "localization.h"
#include "charEncoding.h"
#include "os_string.h"
#include "os_wtoi.h"
#include "os_string.h"
}

static char* replaceAndCountLines(const char* _pstInput, int* _piLines, int* _piNewLine);
static char* addl(TokenDef* token);
static void updatel(TokenDef* token);

#define NanString "Nan"
#define InfString "Inf"
#define NegInfString "-Inf"

char** scilab_sprintf(const std::string& funcname, const char* _pwstInput, types::typed_list &in, int* _piOutputRows, int* _piNewLine)
{
    /* Force Windows display to have two-digit exponent. */
#ifdef _MSC_VER
    _set_output_format(_TWO_DIGIT_EXPONENT);
#endif
    char** pstOutput = nullptr;
    int rhs = in.size();
    char* pstFirstOutput = nullptr;
    *_piNewLine = 0;
    int col = 0;

    int first = 1;
    if (funcname == "mfprintf")
    {
        first = 2;
    }

    //compute couple (index in input and col number ).
    std::list<std::pair<int, int> > inPos;
    for (int i = first; i < in.size(); ++i)
    {
        types::GenericType* gt = in[i]->getAs<types::GenericType>();
        int col = gt->getCols();
        for (int j = 0; j < col; ++j)
        {
            inPos.emplace_back(i, j);
        }
    }

    std::list<std::pair<int, int> >::iterator itPos = inPos.begin();

    //\n \n\r \r \t to string
    //find number of lines
    // replace \\n \\t... by \n \t...
    pstFirstOutput = replaceAndCountLines(_pwstInput, _piOutputRows, _piNewLine);

    std::list<TokenDef*> token;

    size_t start = 0;
    size_t end = 0;
    bool finish = false;

    char* pstStart = pstFirstOutput;
    bool percentpercent = false;

    while (finish == false)
    {
        char* pstEnd = strstr(pstStart + (token.size() == 0 ? 0 : 1), "%");
        start = pstStart - pstFirstOutput;
        percentpercent = false;
        if (pstEnd != nullptr)
        {
            if (token.size() && pstStart[1] == '%')
            {
                //manage "%%"
                pstEnd = strstr(pstEnd + 1, "%");
                if (pstEnd == nullptr)
                {
                    //end of string
                    end = strlen(pstFirstOutput);
                    finish = true;
                }
                else
                {
                    end = pstEnd - pstFirstOutput;
                }

                // skip the first %
                start++;
                percentpercent = true;
            }
            else
            {
                end = pstEnd - pstFirstOutput;
            }
        }
        else
        {
            //end of string
            end = strlen(pstFirstOutput);
            finish = true;
        }

        TokenDef* tok = new TokenDef;
        tok->pstToken = new char[end - start + 1];
        strncpy(tok->pstToken, pstFirstOutput + start, end - start);
        tok->pstToken[end - start] = L'\0';
        token.push_back(tok);

        char* pstPercent = strstr(tok->pstToken, "%");
        if (pstPercent != nullptr && percentpercent == false)
        {
            //looking for flags
            if (*(pstPercent + 1) == '-' ||
                    *(pstPercent + 1) == '+' ||
                    *(pstPercent + 1) == ' ' ||
                    *(pstPercent + 1) == '#' ||
                    *(pstPercent + 1) == '0')
            {
                pstPercent++;
            }

            //looking for width
            if (*(pstPercent + 1) == '*')
            {
                if (itPos == inPos.end())
                {
                    Scierror(999, _("%s: Wrong number of input arguments: data doesn't fit with format.\n"), funcname.data());
                    *_piOutputRows = 0;
                    return nullptr;
                }

                int p = (*itPos).first;
                //input data use to set width
                if (in[p]->getId() != types::InternalType::IdScalarDouble)
                {
                    Scierror(999, _("%s: Wrong type of input arguments #%d: A real scalar expected.\n"), funcname.data(), p);
                    *_piOutputRows = 0;
                    return nullptr;
                }


                types::Double* dbl = in[p]->getAs<types::Double>();
                tok->width = static_cast<int>(dbl->get()[0]);
                tok->widthStar = true;
                ++itPos;
                ++pstPercent;
            }
            else
            {
                //number
                if (isdigit(*(pstPercent + 1)))
                {
                    tok->width = atoi(pstPercent + 1);
                    while (isdigit(*(pstPercent + 1)))
                    {
                        pstPercent++;
                    }
                }
            }

            //looking for precision
            if (*(pstPercent + 1) == '.')
            {
                pstPercent++;

                if (isdigit(*(pstPercent + 1)))
                {
                    tok->prec = atoi(pstPercent + 1);
                    while (isdigit(*(pstPercent + 1)))
                    {
                        pstPercent++;
                    }
                }
                else if (*(pstPercent + 1) == '*')
                {
                    if (itPos == inPos.end())
                    {
                        Scierror(999, _("%s: Wrong number of input arguments: data doesn't fit with format.\n"), funcname.data());
                        *_piOutputRows = 0;
                        return nullptr;
                    }

                    int p = (*itPos).first;
                    //input data use to set prec
                    if (in[p]->getId() != types::InternalType::IdScalarDouble)
                    {
                        Scierror(999, _("%s: Wrong type of input arguments #%d: A real scalar expected.\n"), funcname.data(), p + 1);
                        *_piOutputRows = 0;
                        return nullptr;
                    }

                    types::Double* dbl = in[p]->getAs<types::Double>();
                    tok->prec = static_cast<int>(dbl->get()[0]);
                    tok->precStar = true;
                    ++itPos;
                    ++pstPercent;
                }
            }

            //looking for length
            if (*(pstPercent + 1) == 'h' ||
                    *(pstPercent + 1) == 'l' ||
                    *(pstPercent + 1) == 'L')
            {
                tok->length = true;
                pstPercent++;
            }

            char type = *(pstPercent + 1);
            tok->typePos = (pstPercent + 1) - tok->pstToken;

            switch (type)
            {
                case 'i': //integer
                case 'd': //integer
                {
                    if (itPos == inPos.end())
                    {
                        FREE(pstFirstOutput);
                        Scierror(999, _("%s: Wrong number of input arguments: data doesn't fit with format.\n"), funcname.data());
                        *_piOutputRows = 0;
                        return nullptr;
                    }

                    int p = (*itPos).first;
                    int c = (*itPos).second;
                    if (in[p]->getType() != types::InternalType::ScilabDouble)
                    {
                        FREE(pstFirstOutput);
                        Scierror(999, _("%s: Wrong number of input arguments: data doesn't fit with format.\n"), funcname.data());
                        *_piOutputRows = 0;
                        return nullptr;
                    }

                    tok->outputType = types::InternalType::ScilabInt32;
                    tok->pos = p;
                    tok->col = c;
                    ++itPos;
                    break;
                }
                case 'o': //octal
                case 'u': //unsigned
                case 'x': //hex
                case 'X': //HEX
                {
                    if (itPos == inPos.end())
                    {
                        FREE(pstFirstOutput);
                        Scierror(999, _("%s: Wrong number of input arguments: data doesn't fit with format.\n"), funcname.data());
                        *_piOutputRows = 0;
                        return nullptr;
                    }

                    int p = (*itPos).first;
                    int c = (*itPos).second;
                    if (in[p]->getType() != types::InternalType::ScilabDouble)
                    {
                        FREE(pstFirstOutput);
                        Scierror(999, _("%s: Wrong number of input arguments: data doesn't fit with format.\n"), funcname.data());
                        *_piOutputRows = 0;
                        return nullptr;
                    }

                    tok->outputType = types::InternalType::ScilabUInt32;
                    tok->pos = p;
                    tok->col = c;
                    ++itPos;
                    break;
                }
                case 'f': //float
                case 'e': //exp
                case 'E': //EXP
                case 'g': //shorter between float or exp
                case 'G': //shorter between float or EXP
                {
                    if (itPos == inPos.end())
                    {
                        FREE(pstFirstOutput);
                        Scierror(999, _("%s: Wrong number of input arguments: data doesn't fit with format.\n"), funcname.data());
                        *_piOutputRows = 0;
                        return nullptr;
                    }

                    int p = (*itPos).first;
                    int c = (*itPos).second;
                    if (in[p]->getType() != types::InternalType::ScilabDouble)
                    {
                        FREE(pstFirstOutput);
                        Scierror(999, _("%s: Wrong number of input arguments: data doesn't fit with format.\n"), funcname.data());
                        *_piOutputRows = 0;
                        return nullptr;
                    }

                    tok->outputType = types::InternalType::ScilabDouble;
                    tok->pos = p;
                    tok->col = c;
                    ++itPos;
                    break;
                }
                case 's':
                case 'c':
                {
                    if (itPos == inPos.end())
                    {
                        FREE(pstFirstOutput);
                        Scierror(999, _("%s: Wrong number of input arguments: data doesn't fit with format.\n"), funcname.data());
                        *_piOutputRows = 0;
                        return nullptr;
                    }

                    int p = (*itPos).first;
                    int c = (*itPos).second;
                    if (in[p]->getType() != types::InternalType::ScilabString)
                    {
                        FREE(pstFirstOutput);
                        Scierror(999, _("%s: Wrong number of input arguments: data doesn't fit with format.\n"), funcname.data());
                        *_piOutputRows = 0;
                        return nullptr;
                    }

                    if (tok->length == false)
                    {
                        updatel(tok);
                    }

                    tok->outputType = types::InternalType::ScilabString;
                    tok->pos = p;
                    tok->col = c;
                    ++itPos;
                    break;
                }
                default:
                    FREE(pstFirstOutput);
                    Scierror(999, _("%s: Wrong number of input arguments: data doesn't fit with format.\n"), funcname.data());
                    *_piOutputRows = 0;
                    return nullptr;
                    break;
            }
        }

        //continue
        pstStart = pstEnd;
    }

    FREE(pstFirstOutput);
    pstFirstOutput = nullptr;

    int iLoop = 1;
    if (token.size() > 1)
    {
        std::list<TokenDef*>::iterator it = std::next(token.begin());
        iLoop = in[(*it)->pos]->getAs<types::GenericType>()->getRows();
        for (; it != token.end(); ++it)
        {
            iLoop = std::min(iLoop, in[(*it)->pos]->getAs<types::GenericType>()->getRows());
        }

        if (*_piNewLine || (*_piOutputRows) > 1)
        {
            (*_piOutputRows) *= iLoop;
        }
    }


    //if ((token.size() - 1) != inPos.size())
    //{
    //    Scierror(999, _("%s: Wrong number of input arguments: at most %d expected.\n"), funcname.data(), token.size() - 1);
    //    *_piOutputRows = 0;
    //    return nullptr;
    //}

    std::ostringstream oFirstOutput;
    for (int j = 0; j < iLoop; j++)
    {
        //copy the 0th token
        TokenDef* f = token.front();
        oFirstOutput << f->pstToken;

        //start at 1, the 0th is always without %
        std::list<TokenDef*>::iterator it = std::next(token.begin());
        for (; it != token.end(); ++it)
        {
            TokenDef* tok = *it;
            switch (tok->outputType)
            {
                case types::InternalType::ScilabDouble:
                {
                    char pstTemp[bsiz];
                    double dblVal = in[tok->pos]->getAs<types::Double>()->get(j, tok->col);

                    if (std::isfinite(dblVal))
                    {
                        if (tok->widthStar)
                        {
                            if (tok->precStar)
                            {
                                os_sprintf(pstTemp, bsiz, tok->pstToken, tok->width, tok->prec, dblVal);
                            }
                            else
                            {
                                os_sprintf(pstTemp, bsiz, tok->pstToken, tok->width, dblVal);
                            }
                        }
                        else
                        {
                            if (tok->precStar)
                            {
                                os_sprintf(pstTemp, bsiz, tok->pstToken, tok->prec, dblVal);
                            }
                            else
                            {
                                os_sprintf(pstTemp, bsiz, tok->pstToken, dblVal);
                            }
                        }
                    }
                    else
                    {
                        char* newToken = addl(tok);

                        if (std::isnan(dblVal))
                        {
                            os_sprintf(pstTemp, bsiz, newToken, NanString);
                        }
                        else if (std::signbit(dblVal))
                        {
                            os_sprintf(pstTemp, bsiz, newToken, NegInfString);
                        }
                        else
                        {
                            os_sprintf(pstTemp, bsiz, newToken, InfString);
                        }

                        delete[] newToken;
                    }

                    oFirstOutput << pstTemp;
                    break;
                }
                case types::InternalType::ScilabInt32:
                {
                    char pstTemp[bsiz];
                    double dblVal = in[tok->pos]->getAs<types::Double>()->get(j, tok->col);
                    int iVal = (int)dblVal;
                    if (std::isfinite(dblVal))
                    {
                        if (tok->widthStar)
                        {
                            if (tok->precStar)
                            {
                                os_sprintf(pstTemp, bsiz, tok->pstToken, tok->width, tok->prec, iVal);
                            }
                            else
                            {
                                os_sprintf(pstTemp, bsiz, tok->pstToken, tok->width, iVal);
                            }
                        }
                        else
                        {
                            if (tok->precStar)
                            {
                                os_sprintf(pstTemp, bsiz, tok->pstToken, tok->prec, iVal);
                            }
                            else
                            {
                                os_sprintf(pstTemp, bsiz, tok->pstToken, iVal);
                            }
                        }
                    }
                    else
                    {
                        char* newToken = addl(tok);

                        if (std::isnan(dblVal))
                        {
                            os_sprintf(pstTemp, bsiz, newToken, NanString);
                        }
                        else
                        {
                            if (std::signbit(dblVal))
                            {
                                os_sprintf(pstTemp, bsiz, newToken, NegInfString);
                            }
                            else
                            {
                                os_sprintf(pstTemp, bsiz, newToken, InfString);
                            }
                        }

                        delete[] newToken;
                    }

                    oFirstOutput << pstTemp;
                    break;
                }
                case types::InternalType::ScilabUInt32:
                {
                    char pstTemp[bsiz];
                    double dblVal = in[tok->pos]->getAs<types::Double>()->get(j, tok->col);
                    unsigned int iVal = (unsigned int)dblVal;

                    if (std::isfinite(dblVal))
                    {
                        if (tok->widthStar)
                        {
                            if (tok->precStar)
                            {
                                os_sprintf(pstTemp, bsiz, tok->pstToken, tok->width, tok->prec, iVal);
                            }
                            else
                            {
                                os_sprintf(pstTemp, bsiz, tok->pstToken, tok->width, iVal);
                            }
                        }
                        else
                        {
                            if (tok->precStar)
                            {
                                os_sprintf(pstTemp, bsiz, tok->pstToken, tok->prec, iVal);
                            }
                            else
                            {
                                os_sprintf(pstTemp, bsiz, tok->pstToken, iVal);
                            }
                        }
                    }
                    else
                    {
                        char* newToken = addl(tok);

                        if (std::isnan(dblVal))
                        {
                            os_sprintf(pstTemp, bsiz, newToken, NanString);
                        }
                        else
                        {
                            if (std::signbit(dblVal))
                            {
                                os_sprintf(pstTemp, bsiz, newToken, NegInfString);
                            }
                            else
                            {
                                os_sprintf(pstTemp, bsiz, newToken, InfString);
                            }
                        }

                        delete[] newToken;
                    }

                    oFirstOutput << pstTemp;
                    break;
                }
                case types::InternalType::ScilabString:
                {
                    char* pstStr = nullptr;
                    std::string NaN = NanString;
                    std::string nInf = NegInfString;
                    std::string pInf = InfString;

                    types::InternalType* it = in[tok->pos];
                    if (it->isDouble() && std::isnan(it->getAs<types::Double>()->get(0)))
                    {
                        pstStr = const_cast<char*>(NaN.c_str());
                    }
                    else if (it->isDouble() && std::isfinite(it->getAs<types::Double>()->get(0)) == false)
                    {
                        if (std::signbit(it->getAs<types::Double>()->get(0)))
                        {
                            pstStr = const_cast<char*>(nInf.c_str());
                        }
                        else
                        {
                            pstStr = const_cast<char*>(pInf.c_str());
                        }
                    }
                    else
                    {
                        pstStr = it->getAs<types::String>()->get(j, tok->col);
                    }

                    int posC = (int)strcspn(tok->pstToken, "c");
                    int posS = (int)strcspn(tok->pstToken, "s");

                    if (posS == 0 || posC == 0)
                    {
                        *_piOutputRows = 0;
                        return nullptr;
                    }

                    bool bC = posC < posS;
                    int len = 1;
                    if (tok->prec)
                    {
                        if (bC == false)
                        {
                            len = std::min(std::abs(tok->prec), (int)strlen(pstStr));
                        }
                    }
                    else
                    {
                        if (bC == false)
                        {
                            len = (int)strlen(pstStr);
                        }
                    }

                    int tokenLen = (int)strlen(tok->pstToken);
                    len += tokenLen;
                    len = std::max(len, std::abs(tok->width));
                    //add len of string after token like "%20s>>>" add space for ">>>"
                    len += (tokenLen - (bC ? posC : posS));
                    char* pstTemp = (char*)MALLOC((len + 1) * sizeof(char));

                    if (bC)
                    {
                        if (tok->widthStar)
                        {
                            os_sprintf(pstTemp, len + 1, tok->pstToken, tok->width, pstStr[0]);
                        }
                        else
                        {
                            os_sprintf(pstTemp, len + 1, tok->pstToken, pstStr[0]);
                        }
                    }
                    else
                    {
                        if (tok->widthStar)
                        {
                            os_sprintf(pstTemp, len + 1, tok->pstToken, tok->width, pstStr);
                        }
                        else
                        {
                            os_sprintf(pstTemp, len + 1, tok->pstToken, pstStr);
                        }
                    }

                    oFirstOutput << pstTemp;
                    FREE(pstTemp);
                    break;
                }
                default:
                    // management of %%
                    oFirstOutput << tok->pstToken;
                    break;
            }
        }
    }

    pstFirstOutput = os_strdup(oFirstOutput.str().c_str());

    for (auto & tok : token)
    {
        delete[] tok->pstToken;
        delete tok;
    }

    pstOutput = (char**)MALLOC((*_piOutputRows) * sizeof(char*));

    size_t iLen = strlen(pstFirstOutput);
    int iStart = 0;
    int j = 0;
    for (int i = 0; i < iLen; i++)
    {
        if (pstFirstOutput[i] == '\n')
        {
            int iSize = i - iStart;
            pstOutput[j] = (char*)MALLOC(sizeof(char) * (iSize + 1));
            strncpy(pstOutput[j], pstFirstOutput + iStart, iSize);
            pstOutput[j][iSize] = '\0';
            iStart = i + 1;
            j++;
        }
    }

    if (j == (*_piOutputRows) - 1)
    {
        pstOutput[j] = os_strdup(pstFirstOutput + iStart);
    }

    FREE(pstFirstOutput);
    return pstOutput;
}
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
// replace "\\n" "\\r" "\\t" "\\r\\n" by '\n' '\r' '\t' '\n'
// count number of lines
// indicate if one '\n' is at the end of string
static char* replaceAndCountLines(const char* _pstInput, int* _piLines, int* _piNewLine)
{
    size_t iInputLen = strlen(_pstInput);
    char* pstFirstOutput = (char*)MALLOC(sizeof(char) * (iInputLen + 1));

    int iPos = 0;
    *_piLines = 1;

    for (int i = 0; i < iInputLen; i++)
    {
        if (_pstInput[i] == '\\')
        {
            if (iInputLen == i + 1)
            {
                continue;
            }

            switch (_pstInput[i + 1])
            {
                case 'n':
                    pstFirstOutput[iPos++] = '\n';
                    (*_piLines)++;
                    i++;
                    break;
                case 'r':
                    if (iInputLen > i + 3 && _pstInput[i + 2] == '\\' && _pstInput[i + 3] == 'n')
                    {
                        pstFirstOutput[iPos++] = '\n';
                        (*_piLines)++;
                        i += 3;
                    }
                    else
                    {
                        pstFirstOutput[iPos++] = '\r';
                        i++;
                    }
                    break;
                case 't':
                    pstFirstOutput[iPos++] = '\t';
                    i++;
                    break;
                case '\\':
                    pstFirstOutput[iPos++] = '\\';
                    i++;
                    break;
                default:
                    break;
            }
        }
        else if (_pstInput[i] == 0x0A) // ascii(10) == "\n"
        {
            pstFirstOutput[iPos++] = '\n';
            (*_piLines)++;
        }
        else
        {
            pstFirstOutput[iPos++] = _pstInput[i];
        }
    }

    // do not count '\n' if it's at the end of string
    // it will be manage by piNewLine
    if (pstFirstOutput[iPos - 1] == '\n')
    {
        (*_piLines)--;
        (*_piNewLine) = 1;
    }

    pstFirstOutput[iPos] = 0;
    return pstFirstOutput;
}
/*--------------------------------------------------------------------------*/
char* addl(TokenDef* token)
{
    //replace %s or %c by %ls or %lc to wide char compatibility
    int iPos = token->typePos;
    int sizeTotal = (int)strlen(token->pstToken);
    char* pstToken = new char[sizeTotal + 2];

    strncpy(pstToken, token->pstToken, iPos);
    pstToken[iPos] = 'l';
    pstToken[iPos + 1] = 's';
    strncpy(&pstToken[iPos + 2], token->pstToken + iPos + 1, sizeTotal - (iPos + 1));
    pstToken[sizeTotal + 1] = '\0';

    return pstToken;
}
/*--------------------------------------------------------------------------*/
void updatel(TokenDef* token)
{
    char* newToken = addl(token);
    delete[] token->pstToken;
    token->pstToken = newToken;
}
