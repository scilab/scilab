/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
 *  Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
 *  Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
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

static wchar_t* replaceAndCountLines(const wchar_t* _pwstInput, int* _piLines, int* _piNewLine);
static wchar_t* addl(TokenDef* token);
static void updatel(TokenDef* token);
static void replace_lu_llu(TokenDef* token);
static void replace_ld_lld(TokenDef* token);

#define NanString L"Nan"
#define InfString L"Inf"
#define NegInfString L"-Inf"

wchar_t** scilab_sprintf(const std::string& funcname, const wchar_t* _pwstInput, types::typed_list &in, int* _piOutputRows, int* _piNewLine)
{
    /* Force Windows display to have two-digit exponent. */
#ifdef _MSC_VER
    _set_output_format(_TWO_DIGIT_EXPONENT);
#endif
    wchar_t** pwstOutput = nullptr;
    int rhs = in.size();
    wchar_t* pwstFirstOutput = nullptr;
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
    pwstFirstOutput = replaceAndCountLines(_pwstInput, _piOutputRows, _piNewLine);

    std::list<TokenDef*> token;

    size_t start = 0;
    size_t end = 0;
    bool finish = false;

    wchar_t* pwstStart = pwstFirstOutput;
    bool percentpercent = false;

    while (finish == false)
    {
        wchar_t* pwstEnd = wcsstr(pwstStart + (token.size() == 0 ? 0 : 1), L"%");
        start = pwstStart - pwstFirstOutput;
        percentpercent = false;
        if (pwstEnd != nullptr)
        {
            if (token.size() && pwstStart[1] == L'%')
            {
                //manage "%%"
                pwstEnd = wcsstr(pwstEnd + 1, L"%");
                if (pwstEnd == nullptr)
                {
                    //end of string
                    end = wcslen(pwstFirstOutput);
                    finish = true;
                }
                else
                {
                    end = pwstEnd - pwstFirstOutput;
                }

                // skip the first %
                start++;
                percentpercent = true;
            }
            else
            {
                end = pwstEnd - pwstFirstOutput;
            }
        }
        else
        {
            //end of string
            end = wcslen(pwstFirstOutput);
            finish = true;
        }

        TokenDef* tok = new TokenDef;
        tok->pwstToken = new wchar_t[end - start + 1];
        wcsncpy(tok->pwstToken, pwstFirstOutput + start, end - start);
        tok->pwstToken[end - start] = L'\0';
        token.push_back(tok);

        wchar_t* pwstPercent = wcsstr(tok->pwstToken, L"%");
        if (pwstPercent != nullptr && percentpercent == false)
        {
            //looking for flags
            if (*(pwstPercent + 1) == L'-' ||
                    *(pwstPercent + 1) == L'+' ||
                    *(pwstPercent + 1) == L' ' ||
                    *(pwstPercent + 1) == L'#' ||
                    *(pwstPercent + 1) == L'0')
            {
                pwstPercent++;
            }

            //looking for width
            if (*(pwstPercent + 1) == L'*')
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
                ++pwstPercent;
            }
            else
            {
                //number
                if (iswdigit(*(pwstPercent + 1)))
                {
                    tok->width = os_wtoi(pwstPercent + 1);
                    while (iswdigit(*(pwstPercent + 1)))
                    {
                        pwstPercent++;
                    }
                }
            }

            //looking for precision
            if (*(pwstPercent + 1) == L'.')
            {
                pwstPercent++;

                if (iswdigit(*(pwstPercent + 1)))
                {
                    tok->prec = os_wtoi(pwstPercent + 1);
                    while (iswdigit(*(pwstPercent + 1)))
                    {
                        pwstPercent++;
                    }
                }
                else if (*(pwstPercent + 1) == L'*')
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
                    ++pwstPercent;
                }
            }

            //looking for length
            if (*(pwstPercent + 1) == L'h' ||
                    *(pwstPercent + 1) == L'l' ||
                    *(pwstPercent + 1) == L'L')
            {
                tok->length = true;
                pwstPercent++;
            }

            wchar_t wcType = *(pwstPercent + 1);
            tok->typePos = (pwstPercent + 1) - tok->pwstToken;

            switch (wcType)
            {
                case L'i': //integer
                case L'd': //integer
                {
                    if (itPos == inPos.end())
                    {
                        FREE(pwstFirstOutput);
                        Scierror(999, _("%s: Wrong number of input arguments: data doesn't fit with format.\n"), funcname.data());
                        *_piOutputRows = 0;
                        return nullptr;
                    }

                    int p = (*itPos).first;
                    int c = (*itPos).second;
                    if (in[p]->getType() != types::InternalType::ScilabDouble)
                    {
                        FREE(pwstFirstOutput);
                        Scierror(999, _("%s: Wrong number of input arguments: data doesn't fit with format.\n"), funcname.data());
                        *_piOutputRows = 0;
                        return nullptr;
                    }

                    tok->outputType = types::InternalType::ScilabInt64;
                    tok->pos = p;
                    tok->col = c;
                    ++itPos;
                    break;
                }
                case L'o': //octal
                case L'u': //unsigned
                case L'x': //hex
                case L'X': //HEX
                {
                    if (itPos == inPos.end())
                    {
                        FREE(pwstFirstOutput);
                        Scierror(999, _("%s: Wrong number of input arguments: data doesn't fit with format.\n"), funcname.data());
                        *_piOutputRows = 0;
                        return nullptr;
                    }

                    int p = (*itPos).first;
                    int c = (*itPos).second;
                    if (in[p]->getType() != types::InternalType::ScilabDouble)
                    {
                        FREE(pwstFirstOutput);
                        Scierror(999, _("%s: Wrong number of input arguments: data doesn't fit with format.\n"), funcname.data());
                        *_piOutputRows = 0;
                        return nullptr;
                    }

                    tok->outputType = types::InternalType::ScilabUInt64;
                    tok->pos = p;
                    tok->col = c;
                    ++itPos;
                    break;
                }
                case L'f': //float
                case L'e': //exp
                case L'E': //EXP
                case L'g': //shorter between float or exp
                case L'G': //shorter between float or EXP
                {
                    if (itPos == inPos.end())
                    {
                        FREE(pwstFirstOutput);
                        Scierror(999, _("%s: Wrong number of input arguments: data doesn't fit with format.\n"), funcname.data());
                        *_piOutputRows = 0;
                        return nullptr;
                    }

                    int p = (*itPos).first;
                    int c = (*itPos).second;
                    if (in[p]->getType() != types::InternalType::ScilabDouble)
                    {
                        FREE(pwstFirstOutput);
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
                case L's':
                case L'c':
                {
                    if (itPos == inPos.end())
                    {
                        FREE(pwstFirstOutput);
                        Scierror(999, _("%s: Wrong number of input arguments: data doesn't fit with format.\n"), funcname.data());
                        *_piOutputRows = 0;
                        return nullptr;
                    }

                    int p = (*itPos).first;
                    int c = (*itPos).second;
                    if (in[p]->getType() != types::InternalType::ScilabString)
                    {
                        FREE(pwstFirstOutput);
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
                    FREE(pwstFirstOutput);
                    Scierror(999, _("%s: Wrong number of input arguments: data doesn't fit with format.\n"), funcname.data());
                    *_piOutputRows = 0;
                    return nullptr;
                    break;
            }
        }

        //continue
        pwstStart = pwstEnd;
    }

    FREE(pwstFirstOutput);
    pwstFirstOutput = nullptr;

    int iLoop = 1;
    if (token.size() > 1 && in.size() > first)
    {
        iLoop = in[first]->getAs<types::GenericType>()->getRows();
        for (int i = first + 1; i < in.size(); ++i)
        {
            iLoop = std::min(iLoop, in[i]->getAs<types::GenericType>()->getRows());
        }
    }

    // count number of output lines in function of \n and size of input args
    (*_piOutputRows) *= iLoop;
    (*_piOutputRows) += *_piNewLine == 1 ? 0 : 1;

    pwstOutput = (wchar_t**)MALLOC((*_piOutputRows) * sizeof(wchar_t*));
    int outputIter = 0;
    std::wostringstream oFirstOutput;
    for (int j = 0; j < iLoop; j++)
    {
        wchar_t* tmpToken = NULL;
        for (auto it = token.begin(); it != token.end();/*no inc*/)
        {
            TokenDef* tok = *it;
            wchar_t* token = tmpToken ? tmpToken : tok->pwstToken;
            // find LF in token
            wchar_t* lf = wcsstr(token, L"\n");
            if(lf)
            {
                // create tkoen as part of current token
                size_t sToken = lf - token;
                token = (wchar_t*)MALLOC((sToken + 1) * sizeof(wchar_t));
                wcsncpy(token, tmpToken ? tmpToken : tok->pwstToken, sToken);
                token[sToken] = L'\0';
            }

            // if tmpToken != NULL: we already have managed the % so manage this (part) token as a string without %
            switch (tmpToken ? types::InternalType::ScilabNull : tok->outputType)
            {
                case types::InternalType::ScilabDouble:
                {
                    wchar_t pwstTemp[bsiz];
                    double dblVal = in[tok->pos]->getAs<types::Double>()->get(j, tok->col);

                    if (std::isfinite(dblVal))
                    {
                        if (tok->widthStar)
                        {
                            if (tok->precStar)
                            {
                                os_swprintf(pwstTemp, bsiz, token, tok->width, tok->prec, dblVal);
                            }
                            else
                            {
                                os_swprintf(pwstTemp, bsiz, token, tok->width, dblVal);
                            }
                        }
                        else
                        {
                            if (tok->precStar)
                            {
                                os_swprintf(pwstTemp, bsiz, token, tok->prec, dblVal);
                            }
                            else
                            {
                                os_swprintf(pwstTemp, bsiz, token, dblVal);
                            }
                        }
                    }
                    else
                    {
                        wchar_t* newToken = addl(tok);

                        if (std::isnan(dblVal))
                        {
                            os_swprintf(pwstTemp, bsiz, newToken, NanString);
                        }
                        else if (std::signbit(dblVal))
                        {
                            os_swprintf(pwstTemp, bsiz, newToken, NegInfString);
                        }
                        else
                        {
                            os_swprintf(pwstTemp, bsiz, newToken, InfString);
                        }

                        delete[] newToken;
                    }

                    oFirstOutput << pwstTemp;
                    break;
                }
                case types::InternalType::ScilabInt64:
                {
                    wchar_t pwstTemp[bsiz];
                    double dblVal = in[tok->pos]->getAs<types::Double>()->get(j, tok->col);
                    long long iVal = (long long)dblVal;

                    if (std::isfinite(dblVal))
                    {
                        replace_ld_lld(tok);
                        if (tok->widthStar)
                        {
                            if (tok->precStar)
                            {
                                os_swprintf(pwstTemp, bsiz, token, tok->width, tok->prec, iVal);
                            }
                            else
                            {
                                os_swprintf(pwstTemp, bsiz, token, tok->width, iVal);
                            }
                        }
                        else
                        {
                            if (tok->precStar)
                            {
                                os_swprintf(pwstTemp, bsiz, token, tok->prec, iVal);
                            }
                            else
                            {
                                os_swprintf(pwstTemp, bsiz, token, iVal);
                            }
                        }
                    }
                    else
                    {
                        wchar_t* newToken = addl(tok);

                        if (std::isnan(dblVal))
                        {
                            os_swprintf(pwstTemp, bsiz, newToken, NanString);
                        }
                        else
                        {
                            if (std::signbit(dblVal))
                            {
                                os_swprintf(pwstTemp, bsiz, newToken, NegInfString);
                            }
                            else
                            {
                                os_swprintf(pwstTemp, bsiz, newToken, InfString);
                            }
                        }

                        delete[] newToken;
                    }

                    oFirstOutput << pwstTemp;
                    break;
                }
                case types::InternalType::ScilabUInt64:
                {
                    wchar_t pwstTemp[bsiz];
                    double dblVal = in[tok->pos]->getAs<types::Double>()->get(j, tok->col);
                    unsigned long long iVal = (unsigned long long)dblVal;

                    if (std::isfinite(dblVal))
                    {
                        replace_lu_llu(tok);
                        if (tok->widthStar)
                        {
                            if (tok->precStar)
                            {
                                os_swprintf(pwstTemp, bsiz, token, tok->width, tok->prec, iVal);
                            }
                            else
                            {
                                os_swprintf(pwstTemp, bsiz, token, tok->width, iVal);
                            }
                        }
                        else
                        {
                            if (tok->precStar)
                            {
                                os_swprintf(pwstTemp, bsiz, token, tok->prec, iVal);
                            }
                            else
                            {
                                os_swprintf(pwstTemp, bsiz, token, iVal);
                            }
                        }
                    }
                    else
                    {
                        wchar_t* newToken = addl(tok);

                        if (std::isnan(dblVal))
                        {
                            os_swprintf(pwstTemp, bsiz, newToken, NanString);
                        }
                        else
                        {
                            if (std::signbit(dblVal))
                            {
                                os_swprintf(pwstTemp, bsiz, newToken, NegInfString);
                            }
                            else
                            {
                                os_swprintf(pwstTemp, bsiz, newToken, InfString);
                            }
                        }

                        delete[] newToken;
                    }

                    oFirstOutput << pwstTemp;
                    break;
                }
                case types::InternalType::ScilabString:
                {
                    wchar_t* pwstStr = nullptr;
                    std::wstring NaN = NanString;
                    std::wstring nInf = NegInfString;
                    std::wstring pInf = InfString;

                    types::InternalType* it = in[tok->pos];
                    if (it->isDouble() && std::isnan(it->getAs<types::Double>()->get(0)))
                    {
                        pwstStr = const_cast<wchar_t*>(NaN.c_str());
                    }
                    else if (it->isDouble() && std::isfinite(it->getAs<types::Double>()->get(0)) == false)
                    {
                        if (std::signbit(it->getAs<types::Double>()->get(0)))
                        {
                            pwstStr = const_cast<wchar_t*>(nInf.c_str());
                        }
                        else
                        {
                            pwstStr = const_cast<wchar_t*>(pInf.c_str());
                        }
                    }
                    else
                    {
                        pwstStr = it->getAs<types::String>()->get(j, tok->col);
                    }

                    int posC = (int)wcscspn(token, L"c");
                    int posS = (int)wcscspn(token, L"s");

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
                            len = std::min(std::abs(tok->prec), (int)wcslen(pwstStr));
                        }
                    }
                    else
                    {
                        if (bC == false)
                        {
                            len = (int)wcslen(pwstStr);
                        }
                    }

                    int tokenLen = (int)wcslen(token);
                    len += tokenLen;
                    len = std::max(len, std::abs(tok->width));
                    //add len of string after token like "%20s>>>" add space for ">>>"
                    len += (tokenLen - (bC ? posC : posS));
                    wchar_t* pwstTemp = (wchar_t*)MALLOC((len + 1) * sizeof(wchar_t));

                    if (bC)
                    {
                        if (tok->widthStar)
                        {
                            os_swprintf(pwstTemp, len + 1, token, tok->width, pwstStr[0]);
                        }
                        else
                        {
                            os_swprintf(pwstTemp, len + 1, token, pwstStr[0]);
                        }
                    }
                    else
                    {
                        if (tok->widthStar)
                        {
                            os_swprintf(pwstTemp, len + 1, token, tok->width, pwstStr);
                        }
                        else
                        {
                            os_swprintf(pwstTemp, len + 1, token, pwstStr);
                        }
                    }

                    oFirstOutput << pwstTemp;
                    FREE(pwstTemp);
                    break;
                }
                default:
                {
                    // management of %%
                    oFirstOutput << token;
                }
            }

            if(lf)
            {
                // write current line
                pwstOutput[outputIter++] = os_wcsdup((wchar_t*)oFirstOutput.str().c_str());
                // clear temporary stream and token
                oFirstOutput.clear();
                oFirstOutput.str(L"");
                FREE(token);
                // skip LF
                tmpToken = ++lf;
                // if not at the end of token, continue with same token
                if(lf - tok->pwstToken < wcslen(tok->pwstToken))
                {
                    continue;
                }
            }

            tmpToken = NULL;
            ++it;
        }
    }

    // write last line
    if (oFirstOutput.str().length())
    {
        pwstOutput[outputIter++] = os_wcsdup((wchar_t*)oFirstOutput.str().c_str());
    }
    // fill in the remaining allocated space with default value
    while (outputIter < *_piOutputRows)
    {
        pwstOutput[outputIter++] = os_wcsdup(L"");
    }
    
    for (auto & tok : token)
    {
        delete[] tok->pwstToken;
        delete tok;
    }

    return pwstOutput;
}
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
// replace "\\n" "\\r" "\\t" "\\r\\n" by '\n' '\r' '\t' '\n'
// count number of lines
// indicate if one '\n' is at the end of string
static wchar_t* replaceAndCountLines(const wchar_t* _pwstInput, int* _piLines, int* _piNewLine)
{
    size_t iInputLen = wcslen(_pwstInput);
    wchar_t* pwstFirstOutput = (wchar_t*)MALLOC(sizeof(wchar_t) * (iInputLen + 1));

    int iPos = 0;
    *_piLines = 0;

    for (int i = 0; i < iInputLen; i++)
    {
        if (_pwstInput[i] == L'\\')
        {
            if (iInputLen == i + 1)
            {
                continue;
            }

            switch (_pwstInput[i + 1])
            {
                case L'n':
                    pwstFirstOutput[iPos++] = L'\n';
                    (*_piLines)++;
                    i++;
                    break;
                case L'r':
                    if (iInputLen > i + 3 && _pwstInput[i + 2] == L'\\' && _pwstInput[i + 3] == L'n')
                    {
                        pwstFirstOutput[iPos++] = L'\n';
                        (*_piLines)++;
                        i += 3;
                    }
                    else
                    {
                        pwstFirstOutput[iPos++] = L'\r';
                        i++;
                    }
                    break;
                case L't':
                    pwstFirstOutput[iPos++] = L'\t';
                    i++;
                    break;
                case L'\\':
                    pwstFirstOutput[iPos++] = L'\\';
                    i++;
                    break;
                default:
                    break;
            }
        }
        else if (_pwstInput[i] == 0x0A) // ascii(10) == "\n"
        {
            pwstFirstOutput[iPos++] = L'\n';
            (*_piLines)++;
        }
        else
        {
            pwstFirstOutput[iPos++] = _pwstInput[i];
        }
    }

    // do not count '\n' if it's at the end of string
    // it will be manage by piNewLine
    if (iPos > 0 && pwstFirstOutput[iPos - 1] == '\n')
    {
        (*_piNewLine) = 1;
    }

    pwstFirstOutput[iPos] = 0;
    return pwstFirstOutput;
}
/*--------------------------------------------------------------------------*/
static wchar_t* addl(TokenDef* token)
{
    //replace %s or %c by %ls or %lc to wide char compatibility
    int iPos = token->typePos;
    int sizeTotal = (int)wcslen(token->pwstToken);
    wchar_t* pwstToken = new wchar_t[sizeTotal + 2];

    wcsncpy(pwstToken, token->pwstToken, iPos);
    pwstToken[iPos] = L'l';
    pwstToken[iPos + 1] = token->pwstToken[iPos];
    wcsncpy(&pwstToken[iPos + 2], token->pwstToken + iPos + 1, sizeTotal - (iPos + 1));
    pwstToken[sizeTotal + 1] = L'\0';

    return pwstToken;
}
/*--------------------------------------------------------------------------*/
static void updatel(TokenDef* token)
{
    wchar_t* newToken = addl(token);
    delete[] token->pwstToken;
    token->pwstToken = newToken;
}
/*--------------------------------------------------------------------------*/
static wchar_t* replace(const wchar_t* s, const wchar_t* r, int pos, const wchar_t* token)
{
    std::wstring h(token);
    //find r
    h.replace(pos - 1, wcslen(s), r);

    wchar_t* res = new wchar_t[h.size() + 1];
    wcscpy(res, h.data());
    return res;
}
/*--------------------------------------------------------------------------*/
static void replace_lu_llu(TokenDef* token)
{
#ifdef _MSC_VER
    if (token->length)
    {
        wchar_t* newToken = replace(L"lu", L"llu", token->typePos, token->pwstToken);
        if (newToken)
        {
            delete[] token->pwstToken;
            token->pwstToken = newToken;
        }
    }
#endif
}
/*--------------------------------------------------------------------------*/
static void replace_ld_lld(TokenDef* token)
{
#ifdef _MSC_VER
    if (token->length)
    {
        wchar_t* newToken = replace(L"ld", L"lld", token->typePos, token->pwstToken);
        if (newToken)
        {
            delete[] token->pwstToken;
            token->pwstToken = newToken;
        }
    }
#endif
}
