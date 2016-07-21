/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Yann COLLETTE
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 * Copyright (C) 2010 - DIGITEO - Bernard HUGUENEY
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include <stdarg.h>
/*--------------------------------------------------------------------------*/
#include "parameters.h"
#include "sci_types.h"
#include "sci_malloc.h"
#include "freeArrayOfString.h"
#include "Scierror.h"
#include "sciprint.h"
#include "api_scilab.h"
#include "localization.h"
#include "os_string.h"
/*--------------------------------------------------------------------------*/
static int commonFindLabel(void* _pvCtx, int * _piAddress, char const * const _pstLabelToFind);
static int commonFindLabelPartial(void* _pvCtx, int * _piAddress, char const * const _pstLabelToFind);
static void int_fill_n(int* dst, size_t n, int v);
static void double_fill_n(double* dst, size_t n, int v);
/*--------------------------------------------------------------------------*/
SciErr initPList(void* _pvCtx, int _iVar, int ** _piAddress)
{
    SciErr _SciErr;
    _SciErr.iErr = 0;
    _SciErr.iMsgCount = 0;
    _SciErr = getVarAddressFromPosition(_pvCtx, _iVar, _piAddress);
    return _SciErr;
}
/*--------------------------------------------------------------------------*/
int checkPList(void* _pvCtx, int * _piAddress)
{
    int nb_param = 0, i = 0, var_type = 0;
    int m_label = 0, n_label = 0;
    int * len_label = NULL;
    char ** label_list = NULL;
    int result = 0;
    SciErr _SciErr;

    _SciErr.iErr = 0;
    _SciErr.iMsgCount = 0;

    _SciErr = getVarType(_pvCtx, _piAddress, &var_type);
    if (var_type != sci_mlist)
    {
        return 0;
    }

    _SciErr = getListItemNumber(_pvCtx, _piAddress, &nb_param);

    if (nb_param != 0)
    {
        _SciErr = getMatrixOfStringInList(_pvCtx, _piAddress, 1, &m_label, &n_label, NULL, NULL);

        len_label = (int *)MALLOC(m_label * n_label * sizeof(int));
        _SciErr = getMatrixOfStringInList(_pvCtx, _piAddress, 1, &m_label, &n_label, len_label, NULL);

        label_list = (char **)MALLOC(m_label * n_label * sizeof(char *));
        for (i = 0; i < n_label * m_label; i++)
        {
            label_list[i] = (char *)MALLOC((len_label[i] + 1) * sizeof(char));
        }
        _SciErr = getMatrixOfStringInList(_pvCtx, _piAddress, 1, &m_label, &n_label, len_label, label_list);
        if (strcmp(label_list[0], "plist") != 0)
        {
            if (len_label)
            {
                FREE(len_label);
                len_label = NULL;
            }
            freeArrayOfString(label_list, m_label * n_label);

            return 0;
        }

        if (len_label)
        {
            FREE(len_label);
            len_label = NULL;
        }
        freeArrayOfString(label_list, m_label * n_label);
    }

    result = 1;

    return result;
}
/*--------------------------------------------------------------------------*/
int hasPartialLabelInPList(void* _pvCtx, int * _piAddress, const char * _pstLabel)
{
    SciErr _SciErr;
    _SciErr.iErr = 0;
    _SciErr.iMsgCount = 0;

    return commonFindLabelPartial(_pvCtx, _piAddress, _pstLabel);
}
/*--------------------------------------------------------------------------*/
int hasLabelInPList(void* _pvCtx, int * _piAddress, const char * _pstLabel)
{
    SciErr _SciErr;
    _SciErr.iErr = 0;
    _SciErr.iMsgCount = 0;

    return commonFindLabel(_pvCtx, _piAddress, _pstLabel);
}
/*--------------------------------------------------------------------------*/
SciErr getIntInPList(void* _pvCtx, int * _piAddress, const char * _pstLabel, int * _piValue, int * _piFound,
                     int _iDefaultValue, int _iLog, enum type_check _eCheck, ...)
{
    int pos_label = 0, i = 0;
    int m_tmp = 0, n_tmp = 0;
    double * tmp_dbl = NULL;
    SciErr _SciErr = sciErrInit();

    pos_label = commonFindLabel(_pvCtx, _piAddress, _pstLabel);
    *_piFound = pos_label;

    if (pos_label != -1)
    {
        _SciErr = getMatrixOfDoubleInList(_pvCtx, _piAddress, pos_label + 1, &m_tmp, &n_tmp, &tmp_dbl);
        if (!_SciErr.iErr)
        {
            *_piValue = (int)tmp_dbl[0];
        }
        else
        {
            if (_iLog)
            {
                sciprint(_("%s: wrong parameter type. %s expected. Return default value %d.\n"), "getIntInPList", "int", _iDefaultValue);
            }
            *_piValue = _iDefaultValue;
        }
    }
    else
    {
        if (_iLog)
        {
            sciprint(_("%s: parameter not found. Return default value %d.\n"), "getIntInPList", _iDefaultValue);
        }
        *_piValue = _iDefaultValue;
    }

    /* Now check parameters */

    if (_eCheck != CHECK_NONE)
    {
        va_list vl;
        int nb_value_to_check = 0;
        int value_to_check = 0;
        int check_res = 0;

        va_start(vl, _eCheck);

        switch (_eCheck)
        {
            case CHECK_MIN:
                value_to_check = va_arg(vl, int);
                va_end(vl);
                if (value_to_check > *_piValue)
                {
                    if ((*_piFound != -1) && (_iLog))
                    {
                        sciprint(_("%s: wrong min bound for parameter %s: min bound %d, value %d\n"), "getIntInPList", _pstLabel, value_to_check, *_piValue);
                    }
                    *_piValue = _iDefaultValue;
                    addErrorMessage(&_SciErr, 999, _("%s: wrong min bound for parameter %s: min bound %d, value %d\n"), "getIntInPList", _pstLabel, value_to_check, *_piValue);
                    return _SciErr;
                }
                break;
            case CHECK_MAX:
                value_to_check = va_arg(vl, int);
                va_end(vl);
                if (value_to_check < *_piValue)
                {
                    if ((*_piFound != -1) && (_iLog))
                    {
                        sciprint(_("%s: wrong max bound for parameter %s: max bound %d, value %d\n"), "getIntInPList", _pstLabel, value_to_check, *_piValue);
                    }
                    *_piValue = _iDefaultValue;
                    addErrorMessage(&_SciErr, 999, _("%s: wrong max bound for parameter %s: max bound %d, value %d\n"), "getIntInPList", _pstLabel, value_to_check, *_piValue);
                    return _SciErr;
                }
                break;
            case CHECK_BOTH:
                // First value is the min bound
                value_to_check = va_arg(vl, int);
                if (value_to_check > *_piValue)
                {
                    if ((*_piFound != -1) && (_iLog))
                    {
                        sciprint(_("%s: wrong min bound for parameter %s: min bound %d, value %d\n"), "getIntInPList", _pstLabel, value_to_check, *_piValue);
                    }
                    *_piValue = _iDefaultValue;
                    va_end(vl);
                    addErrorMessage(&_SciErr, 999, _("%s: wrong min bound for parameter %s: min bound %d, value %d\n"), "getIntInPList", _pstLabel, value_to_check, *_piValue);
                    return _SciErr;
                }
                // Second value is the max bound
                value_to_check = va_arg(vl, int);
                va_end(vl);
                if (value_to_check < *_piValue)
                {
                    if ((*_piFound != -1) && (_iLog))
                    {
                        sciprint(_("%s: wrong max bound for parameter %s: max bound %d, value %d\n"), "getIntInPList", _pstLabel, value_to_check, *_piValue);
                    }
                    *_piValue = _iDefaultValue;
                    addErrorMessage(&_SciErr, 999, _("%s: wrong max bound for parameter %s: max bound %d, value %d\n"), "getIntInPList", _pstLabel, value_to_check, *_piValue);
                    return _SciErr;
                }
                break;
            case CHECK_VALUES:
                // First parameters is int and contains the number of values to check
                nb_value_to_check = va_arg(vl, int);
                check_res = 0;
                for (i = 0; i < nb_value_to_check; i++)
                {
                    value_to_check = va_arg(vl, int);
                    check_res = check_res || (value_to_check == *_piValue);
                }

                if (!check_res)
                {
                    if ((*_piFound != -1) && (_iLog))
                    {
                        sciprint(_("%s: wrong value for parameter %s: value %d\n"), "getIntInPList", _pstLabel, *_piValue);
                        sciprint(_("%s: awaited parameters: "), "getIntInPList");
                        va_start(vl, _eCheck);
                        nb_value_to_check = va_arg(vl, int);
                        for (i = 0; i < nb_value_to_check; i++)
                        {
                            value_to_check = va_arg(vl, int);
                            sciprint(" %d", value_to_check);
                        }
                        sciprint("\n");
                    }

                    *_piValue = _iDefaultValue;

                    va_end(vl);
                    addErrorMessage(&_SciErr, 999, _("%s: wrong value for parameter %s: value %d\n"), "getIntInPList", _pstLabel, *_piValue);
                    return _SciErr;
                }

                va_end(vl);
                break;
            default:
                va_end(vl);
        }
    }
    return _SciErr;
}
/*--------------------------------------------------------------------------*/
SciErr getDoubleInPList(void* _pvCtx, int * _piAddress, const char * _pstLabel, double * _pdblValue, int * _piFound,
                        double _dblDefaultValue, int _iLog, enum type_check _eCheck, ...)
{
    int pos_label = 0, i = 0;
    int m_tmp = 0, n_tmp = 0;
    double * tmp_values = NULL;
    SciErr _SciErr = sciErrInit();

    pos_label = commonFindLabel(_pvCtx, _piAddress, _pstLabel);
    *_piFound = pos_label;

    if (pos_label != -1)
    {
        _SciErr = getMatrixOfDoubleInList(_pvCtx, _piAddress, pos_label + 1, &m_tmp, &n_tmp, &tmp_values);
        if (!_SciErr.iErr)
        {
            *_pdblValue = tmp_values[0];
        }
        else
        {
            if (_iLog)
            {
                sciprint(_("%s: wrong parameter type. %s expected. Return default value %f.\n"), "getDoubleInPList", "double", _dblDefaultValue);
            }
            *_pdblValue = _dblDefaultValue;
        }
    }
    else
    {
        if (_iLog)
        {
            sciprint(_("%s: parameter not found. Return default value %f.\n"), "getDoubleInPList", _dblDefaultValue);
        }
        *_pdblValue = _dblDefaultValue;
    }

    /* Now check parameters */

    if (_eCheck != CHECK_NONE)
    {
        va_list vl;
        int nb_value_to_check = 0;
        double value_to_check = 0;
        int check_res = 0;

        va_start(vl, _eCheck);

        switch (_eCheck)
        {
            case CHECK_MIN:
                value_to_check = va_arg(vl, double);
                va_end(vl);
                if (value_to_check > *_pdblValue)
                {
                    if ((*_piFound != -1) && (_iLog))
                    {
                        sciprint(_("%s: wrong min bound for parameter %s: min bound %f, value %f\n"), "getDoubleInPList", _pstLabel, value_to_check, *_pdblValue);
                    }
                    *_pdblValue = _dblDefaultValue;
                    addErrorMessage(&_SciErr, 999, _("%s: wrong min bound for parameter %s: min bound %f, value %f\n"), "getDoubleInPList", _pstLabel, value_to_check, *_pdblValue);
                    return _SciErr;
                }
                break;
            case CHECK_MAX:
                value_to_check = va_arg(vl, double);
                va_end(vl);
                if (value_to_check < *_pdblValue)
                {
                    if ((*_piFound != -1) && (_iLog))
                    {
                        sciprint(_("%s: wrong max bound for parameter %s: max bound %f, value %f\n"), "getDoubleInPList", _pstLabel, value_to_check, *_pdblValue);
                    }
                    *_pdblValue = _dblDefaultValue;
                    addErrorMessage(&_SciErr, 999, _("%s: wrong max bound for parameter %s: max bound %f, value %f\n"), "getDoubleInPList", _pstLabel, value_to_check, *_pdblValue);
                    return _SciErr;
                }
                break;
            case CHECK_BOTH:
                /* First value is the min bound */
                value_to_check = va_arg(vl, double);
                if (value_to_check > *_pdblValue)
                {
                    if ((*_piFound != -1) && (_iLog))
                    {
                        sciprint(_("%s: wrong min bound for parameter %s: min bound %f, value %f\n"), "getDoubleInPList", _pstLabel, value_to_check, *_pdblValue);
                    }
                    *_pdblValue = _dblDefaultValue;
                    va_end(vl);
                    addErrorMessage(&_SciErr, 999, _("%s: wrong min bound for parameter %s: min bound %f, value %f\n"), "getDoubleInPList", _pstLabel, value_to_check, *_pdblValue);
                    return _SciErr;
                }
                /* Second value is the max bound */
                value_to_check = va_arg(vl, double);
                va_end(vl);
                if (value_to_check < *_pdblValue)
                {
                    if ((*_piFound != -1) && (_iLog))
                    {
                        sciprint(_("%s: wrong max bound for parameter %s: max bound %f, value %f\n"), "getDoubleInPList", _pstLabel, value_to_check, *_pdblValue);
                    }
                    *_pdblValue = _dblDefaultValue;
                    addErrorMessage(&_SciErr, 999, _("%s: wrong max bound for parameter %s: max bound %f, value %f\n"), "getDoubleInPList", _pstLabel, value_to_check, *_pdblValue);
                    return _SciErr;
                }
                break;
            case CHECK_VALUES:
                /* First parameters is int and contains the number of values to check */
                nb_value_to_check = va_arg(vl, int);
                check_res = 0;
                for (i = 0; i < nb_value_to_check; i++)
                {
                    value_to_check = va_arg(vl, double);
                    check_res = check_res || (value_to_check == *_pdblValue);
                }

                if (!check_res)
                {
                    if ((*_piFound != -1) && (_iLog))
                    {
                        sciprint(_("%s: wrong value for parameter %s: value %f\n"), "getDoubleInPList", _pstLabel, *_pdblValue);
                        sciprint(_("%s: awaited parameters: "), "getDoubleInPList");
                        va_start(vl, _eCheck);
                        nb_value_to_check = va_arg(vl, int);
                        for (i = 0; i < nb_value_to_check; i++)
                        {
                            value_to_check = va_arg(vl, double);
                            sciprint(" %f", value_to_check);
                        }
                        sciprint("\n");
                    }

                    *_pdblValue = _dblDefaultValue;

                    va_end(vl);
                    addErrorMessage(&_SciErr, 999, _("%s: wrong value for parameter %s: value %f\n"), "getDoubleInPList", _pstLabel, *_pdblValue);
                    return _SciErr;
                }
                va_end(vl);
                break;
            default:
                va_end(vl);
        }
    }

    return _SciErr;
}
/*--------------------------------------------------------------------------*/
SciErr getStringInPList(void* _pvCtx, int * _piAddress, const char * _pstLabel, char ** _pstValue, int * _piFound,
                        const char * _pstDefaultValue, int _iLog, enum type_check _eCheck, ...)
{
    int pos_label = 0, i = 0;
    int m_label = 0, n_label = 0;
    int * len_label = NULL;
    char ** label_list = NULL;
    SciErr _SciErr = sciErrInit();

    pos_label = commonFindLabel(_pvCtx, _piAddress, _pstLabel);
    *_piFound = pos_label;

    if (pos_label != -1)
    {
        _SciErr = getMatrixOfStringInList(_pvCtx, _piAddress, pos_label + 1, &m_label, &n_label, NULL, NULL);
        len_label = (int *)MALLOC(m_label * n_label * sizeof(int));
        _SciErr = getMatrixOfStringInList(_pvCtx, _piAddress, pos_label + 1, &m_label, &n_label, len_label, NULL);
        label_list = (char **)MALLOC(m_label * n_label * sizeof(char *));
        for (i = 0; i < n_label * m_label; i++)
        {
            label_list[i] = (char *)MALLOC((len_label[i] + 1) * sizeof(char));
        }
        _SciErr = getMatrixOfStringInList(_pvCtx, _piAddress, pos_label + 1, &m_label, &n_label, len_label, label_list);

        if (!_SciErr.iErr)
        {
            if (label_list[0])
            {
                *_pstValue = os_strdup(label_list[0]);
            }
            else
            {
                if (_iLog)
                {
                    sciprint(_("%s: wrong parameter type. %s expected. Return default value %s.\n"), "getStringInPList", "string", _pstDefaultValue);
                }
                *_pstValue = os_strdup(_pstDefaultValue);
            }
        }
        else
        {
            if (_iLog)
            {
                sciprint(_("%s: parameter not found. Return default value %s.\n"), "getStringInPList", _pstDefaultValue);
            }
            *_pstValue = os_strdup(_pstDefaultValue);
        }

        if (len_label)
        {
            FREE(len_label);
            len_label = NULL;
        }
        freeArrayOfString(label_list, m_label * n_label);
    }
    else
    {
        *_pstValue = os_strdup(_pstDefaultValue);
    }

    /* Now check parameters */

    if (_eCheck != CHECK_NONE)
    {
        va_list vl;
        int nb_value_to_check = 0;
        char * value_to_check = 0;
        int check_res = 0;

        va_start(vl, _eCheck);

        switch (_eCheck)
        {
            case CHECK_VALUES:
                /* First parameters is int and contains the number of values to check */
                nb_value_to_check = va_arg(vl, int);
                check_res = 0;
                for (i = 0; i < nb_value_to_check; i++)
                {
                    value_to_check = va_arg(vl, char *);
                    check_res = check_res || (strcmp(value_to_check, *_pstValue) == 0);
                }

                if (!check_res)
                {
                    if ((*_piFound != -1) && (_iLog))
                    {
                        sciprint(_("%s: wrong value for parameter %s: value %s\n"), "getStringInPList", _pstLabel, *_pstValue);
                        sciprint(_("%s: awaited parameters: "), "getStringInPList");
                        va_start(vl, _eCheck);
                        nb_value_to_check = va_arg(vl, int);
                        for (i = 0; i < nb_value_to_check; i++)
                        {
                            value_to_check = va_arg(vl, char *);
                            sciprint(" \"%s\"", value_to_check);
                        }
                        sciprint("\n");
                    }

                    if (*_pstValue)
                    {
                        FREE(*_pstValue);
                        *_pstValue = NULL;
                    }

                    *_pstValue = os_strdup(_pstDefaultValue);

                    va_end(vl);
                    addErrorMessage(&_SciErr, 999, _("%s: wrong value for parameter %s: value %s\n"), "getStringInPList", _pstLabel, *_pstValue);
                    return _SciErr;
                }

                va_end(vl);
                break;
            default:
                va_end(vl);
        }
    }

    return _SciErr;
}
/*--------------------------------------------------------------------------*/
/* get vector of double / integers */
SciErr getColVectorOfIntInPList(void* _pvCtx, int * _piAddress, const char * _pstLabel, int * _piValue, int * _piFound,
                                int _iDefaultValue, int _iDefaultSize, int * _piSize, int _iLog, enum type_check _eCheck, ...)
{
    int pos_label = 0, i = 0;
    int m_tmp = 0, n_tmp = 0;
    double * tmp_dbl = 0;
    SciErr _SciErr = sciErrInit();

    *_piSize = -1;

    pos_label = commonFindLabel(_pvCtx, _piAddress, _pstLabel);
    *_piFound = pos_label;

    if (pos_label != -1)
    {
        _SciErr = getMatrixOfDoubleInList(_pvCtx, _piAddress, pos_label + 1, &m_tmp, &n_tmp, &tmp_dbl);
        if (!_SciErr.iErr)
        {
            *_piSize  = m_tmp * n_tmp;
            for (i = 0; i < *_piSize; i++)
            {
                _piValue[i] = (int)tmp_dbl[i];
            }
        }
        else
        {
            if (_iLog)
            {
                sciprint(_("%s: wrong parameter type. %s expected. Return default value %d.\n"), "getColVectorOfIntInPList", "int", _iDefaultValue);
            }
            *_piSize  = _iDefaultSize;
            int_fill_n(_piValue, _iDefaultSize, _iDefaultValue);
        }
    }
    else
    {
        if (_iLog)
        {
            sciprint(_("%s: parameter not found. Return default value %d.\n"), "getColVectorOfIntInPList", _iDefaultValue);
        }
        *_piSize  = _iDefaultSize;
        int_fill_n(_piValue, _iDefaultSize, _iDefaultValue);
    }

    /* Now check parameters */

    if (_eCheck != CHECK_NONE)
    {
        va_list vl;
        int nb_value_to_check = 0;
        int value_to_check = 0;
        int check_res = 0;

        va_start(vl, _eCheck);

        switch (_eCheck)
        {
            case CHECK_SIZE:
                value_to_check = va_arg(vl, int);
                va_end(vl);
                if (value_to_check != *_piSize)
                {
                    if ((*_piFound != -1) && (_iLog))
                    {
                        sciprint(_("%s: wrong size for parameter %s: %d requested, got %d\n"), "getColVectorOfIntInPList", _pstLabel, value_to_check, *_piSize);
                    }
                    int_fill_n(_piValue, _iDefaultSize, _iDefaultValue);
                    addErrorMessage(&_SciErr, 999, _("%s: wrong size for parameter %s: %d requested, got %d\n"), "getColVectorOfIntInPList", _pstLabel, value_to_check, *_piSize);
                    return _SciErr;
                }
                break;
            case CHECK_MIN:
                value_to_check = va_arg(vl, int);
                va_end(vl);
                if (value_to_check > *_piValue)
                {
                    if ((*_piFound != -1) && (_iLog))
                    {
                        sciprint(_("%s: wrong min bound for parameter %s: min bound %d, value %d\n"), "getColVectorOfIntInPList", _pstLabel, value_to_check, *_piValue);
                    }
                    int_fill_n(_piValue, _iDefaultSize, _iDefaultValue);
                    addErrorMessage(&_SciErr, 999, _("%s: wrong min bound for parameter %s: min bound %d, value %d\n"), "getColVectorOfIntInPList", _pstLabel, value_to_check, *_piValue);
                    return _SciErr;
                }
                break;
            case CHECK_MAX:
                value_to_check = va_arg(vl, int);
                va_end(vl);
                if (value_to_check < *_piValue)
                {
                    if ((*_piFound != -1) && (_iLog))
                    {
                        sciprint(_("%s: wrong max bound for parameter %s: max bound %d, value %d\n"), "getColVectorOfIntInPList", _pstLabel, value_to_check, *_piValue);
                    }
                    int_fill_n(_piValue, _iDefaultSize, _iDefaultValue);
                    addErrorMessage(&_SciErr, 999, _("%s: wrong max bound for parameter %s: max bound %d, value %d\n"), "getColVectorOfIntInPList", _pstLabel, value_to_check, *_piValue);
                    return _SciErr;
                }
                break;
            case CHECK_BOTH:
                /* First value is the min bound */
                value_to_check = va_arg(vl, int);
                if (value_to_check > *_piValue)
                {
                    if ((*_piFound != -1) && (_iLog))
                    {
                        sciprint(_("%s: wrong min bound for parameter %s: min bound %d, value %d\n"), "getColVectorOfIntInPList", _pstLabel, value_to_check, *_piValue);
                    }
                    int_fill_n(_piValue, _iDefaultSize, _iDefaultValue);
                    va_end(vl);
                    addErrorMessage(&_SciErr, 999, _("%s: wrong min bound for parameter %s: min bound %d, value %d\n"), "getColVectorOfIntInPList", _pstLabel, value_to_check, *_piValue);
                    return _SciErr;
                }
                /* Second value is the max bound */
                value_to_check = va_arg(vl, int);
                va_end(vl);
                if (value_to_check < *_piValue)
                {
                    if ((*_piFound != -1) && (_iLog))
                    {
                        sciprint(_("%s: wrong max bound for parameter %s: max bound %d, value %d\n"), "getColVectorOfIntInPList", _pstLabel, value_to_check, *_piValue);
                    }
                    int_fill_n(_piValue, _iDefaultSize, _iDefaultValue);
                    addErrorMessage(&_SciErr, 999, _("%s: wrong max bound for parameter %s: max bound %d, value %d\n"), "getColVectorOfIntInPList", _pstLabel, value_to_check, *_piValue);
                    return _SciErr;
                }
                break;
            case CHECK_VALUES:
                /* First parameters is int and contains the number of values to check */
                nb_value_to_check = va_arg(vl, int);
                check_res = 0;
                for (i = 0; i < nb_value_to_check; i++)
                {
                    value_to_check = va_arg(vl, int);
                    check_res = check_res || (value_to_check == *_piValue);
                }

                if (!check_res)
                {
                    if ((*_piFound != -1) && (_iLog))
                    {
                        sciprint(_("%s: wrong value for parameter %s: value %d\n"), "getColVectorOfIntInPList", _pstLabel, *_piValue);
                        sciprint(_("%s: awaited parameters: "), "getColVectorOfIntInPList");
                        va_start(vl, _eCheck);
                        nb_value_to_check = va_arg(vl, int);
                        for (i = 0; i < nb_value_to_check; i++)
                        {
                            value_to_check = va_arg(vl, int);
                            sciprint(" %d", value_to_check);
                        }
                        sciprint("\n");
                    }

                    int_fill_n(_piValue, _iDefaultSize, _iDefaultValue);

                    va_end(vl);
                    addErrorMessage(&_SciErr, 999, _("%s: wrong value for parameter %s: value %d\n"), "getColVectorOfIntInPList", _pstLabel, *_piValue);
                    return _SciErr;
                }

                va_end(vl);
                break;
            default:
                va_end(vl);
        }
    }

    return _SciErr;
}

SciErr getColVectorOfDoubleInPList(void* _pvCtx, int * _piAddress, const char * _pstLabel, double * _pdblValue, int * _piFound,
                                   double _dblDefaultValue, int _iDefaultSize, int * _piSize, int _iLog, enum type_check _eCheck, ...)
{
    int pos_label = 0, i = 0;
    int m_label = 0, n_label = 0;
    int m_tmp = 0, n_tmp = 0;
    double * tmp_values = NULL;
    char ** label_list = NULL;
    SciErr _SciErr = sciErrInit();

    *_piSize = -1;

    pos_label = commonFindLabel(_pvCtx, _piAddress, _pstLabel);
    *_piFound = pos_label;

    freeArrayOfString(label_list, m_label * n_label);

    if (pos_label != -1)
    {
        _SciErr = getMatrixOfDoubleInList(_pvCtx, _piAddress, pos_label + 1, &m_tmp, &n_tmp, &tmp_values);
        if (!_SciErr.iErr)
        {
            *_piSize  = m_tmp * n_tmp;

            memcpy( _pdblValue,  tmp_values, sizeof(double) * (*_piSize));
        }
        else
        {
            if (_iLog)
            {
                sciprint(_("%s: wrong parameter type. %s expected. Return default value %f.\n"), "getColVectorOfDoubleInPList", "double", _dblDefaultValue);
            }
            *_piSize  = _iDefaultSize;
            double_fill_n(_pdblValue, (size_t)_iDefaultSize, (int)_dblDefaultValue);
        }
    }
    else
    {
        if (_iLog)
        {
            sciprint(_("%s: parameter not found. Return default value %f.\n"), "getColVectorOfDoubleInPList", _dblDefaultValue);
        }
        *_piSize  = _iDefaultSize;
        double_fill_n(_pdblValue, (size_t)_iDefaultSize, (int)_dblDefaultValue);
    }

    /* Now check parameters */

    if (_eCheck != CHECK_NONE)
    {
        va_list vl;
        int nb_value_to_check = 0;
        double value_to_check = 0;
        int check_res = 0;

        va_start(vl, _eCheck);

        switch (_eCheck)
        {
            case CHECK_SIZE:
                value_to_check = va_arg(vl, double);
                va_end(vl);
                if (value_to_check != *_piSize)
                {
                    if ((*_piFound != -1) && (_iLog))
                    {
                        sciprint(_("%s: wrong size for parameter %s: %d requested, got %d\n"), "getColVectorOfDoubleInPList", _pstLabel, value_to_check, *_piSize);
                    }

                    for (i = 0; i < _iDefaultSize; i++)
                    {
                        _pdblValue[i] = _dblDefaultValue;
                    }

                    addErrorMessage(&_SciErr, 999, _("%s: wrong size for parameter %s: %d requested, got %d\n"), "getColVectorOfDoubleInPList", _pstLabel, value_to_check, *_piSize);
                    return _SciErr;
                }
                break;
            case CHECK_MIN:
                value_to_check = va_arg(vl, double);
                va_end(vl);
                if (value_to_check > *_pdblValue)
                {
                    if ((*_piFound != -1) && (_iLog))
                    {
                        sciprint(_("%s: wrong min bound for parameter %s: min bound %f, value %f\n"), "getColVectorOfDoubleInPList", _pstLabel, value_to_check, *_pdblValue);
                    }

                    for (i = 0; i < _iDefaultSize; i++)
                    {
                        _pdblValue[i] = _dblDefaultValue;
                    }

                    addErrorMessage(&_SciErr, 999, _("%s: wrong min bound for parameter %s: min bound %f, value %f\n"), "getColVectorOfDoubleInPList", _pstLabel, value_to_check, *_pdblValue);
                    return _SciErr;
                }
                break;
            case CHECK_MAX:
                value_to_check = va_arg(vl, double);
                va_end(vl);
                if (value_to_check < *_pdblValue)
                {
                    if ((*_piFound != -1) && (_iLog))
                    {
                        sciprint(_("%s: wrong max bound for parameter %s: max bound %f, value %f\n"), "getColVectorOfDoubleInPList", _pstLabel, value_to_check, *_pdblValue);
                    }

                    for (i = 0; i < _iDefaultSize; i++)
                    {
                        _pdblValue[i] = _dblDefaultValue;
                    }

                    addErrorMessage(&_SciErr, 999, _("%s: wrong max bound for parameter %s: max bound %f, value %f\n"), "getColVectorOfDoubleInPList", _pstLabel, value_to_check, *_pdblValue);
                    return _SciErr;
                }
                break;
            case CHECK_BOTH:
                /* First value is the min bound */
                value_to_check = va_arg(vl, double);
                if (value_to_check > *_pdblValue)
                {
                    if ((*_piFound != -1) && (_iLog))
                    {
                        sciprint(_("%s: wrong min bound for parameter %s: min bound %f, value %f\n"), "getColVectorOfDoubleInPList", _pstLabel, value_to_check, *_pdblValue);
                    }

                    for (i = 0; i < _iDefaultSize; i++)
                    {
                        _pdblValue[i] = _dblDefaultValue;
                    }

                    va_end(vl);
                    addErrorMessage(&_SciErr, 999, _("%s: wrong min bound for parameter %s: min bound %f, value %f\n"), "getColVectorOfDoubleInPList", _pstLabel, value_to_check, *_pdblValue);
                    return _SciErr;
                }
                /* Second value is the max bound */
                value_to_check = va_arg(vl, double);
                va_end(vl);
                if (value_to_check < *_pdblValue)
                {
                    if ((*_piFound != -1) && (_iLog))
                    {
                        sciprint(_("%s: wrong max bound for parameter %s: max bound %f, value %f\n"), "getColVectorOfDoubleInPList", _pstLabel, value_to_check, *_pdblValue);
                    }

                    for (i = 0; i < _iDefaultSize; i++)
                    {
                        _pdblValue[i] = _dblDefaultValue;
                    }

                    addErrorMessage(&_SciErr, 999, _("%s: wrong max bound for parameter %s: max bound %f, value %f\n"), "getColVectorOfDoubleInPList", _pstLabel, value_to_check, *_pdblValue);
                    return _SciErr;
                }
                break;
            case CHECK_VALUES:
                /* First parameters is int and contains the number of values to check */
                nb_value_to_check = va_arg(vl, int);
                check_res = 0;
                for (i = 0; i < nb_value_to_check; i++)
                {
                    value_to_check = va_arg(vl, double);
                    check_res = check_res || (value_to_check == *_pdblValue);
                }
                if (!check_res)
                {
                    if ((*_piFound != -1) && (_iLog))
                    {
                        sciprint(_("%s: wrong value for parameter %s: value %f\n"), "getColVectorOfDoubleInPList", _pstLabel, *_pdblValue);
                        sciprint(_("%s: awaited parameters: "), "getColVectorOfDoubleInPList");
                        va_start(vl, _eCheck);
                        nb_value_to_check = va_arg(vl, int);
                        for (i = 0; i < nb_value_to_check; i++)
                        {
                            value_to_check = va_arg(vl, double);
                            sciprint(" %f", value_to_check);
                        }
                        sciprint("\n");
                    }

                    for (i = 0; i < _iDefaultSize; i++)
                    {
                        _pdblValue[i] = _dblDefaultValue;
                    }
                    va_end(vl);
                    addErrorMessage(&_SciErr, 999, _("%s: wrong value for parameter %s: value %f\n"), "getColVectorOfDoubleInPList", _pstLabel, *_pdblValue);
                    return _SciErr;
                }
                va_end(vl);
                break;
            default:
                va_end(vl);
        }
    }

    return _SciErr;
}
/*--------------------------------------------------------------------------*/
SciErr createPList(void* _pvCtx, int _iVar, int ** _piAddress, char ** _pstLabelNames, int _iNbParams)
{
    SciErr _SciErr;
    int i = 0;
    char ** label_list = NULL;

    _SciErr.iErr = 0;
    _SciErr.iMsgCount = 0;

    _SciErr = createMList(_pvCtx, _iVar, _iNbParams + 1, _piAddress);
    label_list = (char **)MALLOC((_iNbParams + 1) * sizeof(char *));
    label_list[0] = os_strdup("plist");

    for (i = 1; i <= _iNbParams; i++)
    {
        label_list[i] = os_strdup(_pstLabelNames[i - 1]);
    }

    _SciErr = createMatrixOfStringInList(_pvCtx, _iVar, *_piAddress, 1, 1, _iNbParams + 1, (char const * const*) label_list);

    freeArrayOfString(label_list, _iNbParams + 1);

    return _SciErr;
}
/*--------------------------------------------------------------------------*/
SciErr createIntInPList(void* _pvCtx, int _iVar, int * _piAddress, char * _pstLabelName, int _iValue)
{
    int itemPos = -1;
    double tmp_val[1];
    SciErr _SciErr;

    _SciErr.iErr = 0;
    _SciErr.iMsgCount = 0;

    itemPos = commonFindLabel(_pvCtx, _piAddress, _pstLabelName) + 1;

#ifdef DEBUG
    sciprint("DEBUG: addIntInPList - itemPos = %d _pstLabelName = %s\n", itemPos, _pstLabelName);
#endif

    tmp_val[0] = (double)_iValue;
    _SciErr = createMatrixOfDoubleInList(_pvCtx, _iVar, _piAddress, itemPos, 1, 1, tmp_val);

    return _SciErr;
}
/*--------------------------------------------------------------------------*/
SciErr createDoubleInPList(void* _pvCtx, int _iVar, int * _piAddress, char * _pstLabelName, double _dblValue)
{
    int itemPos = -1;
    double tmp_val[1];
    SciErr _SciErr;

    _SciErr.iErr = 0;
    _SciErr.iMsgCount = 0;

    itemPos = commonFindLabel(_pvCtx, _piAddress, _pstLabelName) + 1;

#ifdef DEBUG
    sciprint("DEBUG: addDoubleInPList - itemPos = %d _pstLabelName = %s\n", itemPos, _pstLabelName);
#endif

    tmp_val[0] = _dblValue;
    _SciErr = createMatrixOfDoubleInList(_pvCtx, _iVar, _piAddress, itemPos, 1, 1, tmp_val);

    return _SciErr;
}
/*--------------------------------------------------------------------------*/
SciErr createStringInPList(void* _pvCtx, int _iVar, int * _piAddress, char * _pstLabelName, char * _pstValue)
{
    int itemPos = -1;
    char * tmp_val[1];
    SciErr _SciErr;

    _SciErr.iErr = 0;
    _SciErr.iMsgCount = 0;

    itemPos = commonFindLabel(_pvCtx, _piAddress, _pstLabelName) + 1;

#ifdef DEBUG
    sciprint("DEBUG: addStringInPList - itemPos = %d _pstLabelName = %s\n", itemPos, _pstLabelName);
#endif

    tmp_val[0] = os_strdup(_pstValue);
    _SciErr = createMatrixOfStringInList(_pvCtx, _iVar, _piAddress, itemPos, 1, 1, (char const * const*) tmp_val);

    if (tmp_val[0])
    {
        FREE(tmp_val[0]);
        tmp_val[0] = NULL;

    }

    return _SciErr;
}
/*--------------------------------------------------------------------------*/
SciErr createColVectorOfIntInPList(void* _pvCtx, int _iVar, int * _piAddress, char * _pstLabelName, int _iNbValues, int * _piValue)
{
    int itemPos = -1, i = 0;
    double * tmp_val = NULL;
    SciErr _SciErr;

    _SciErr.iErr = 0;
    _SciErr.iMsgCount = 0;

    itemPos = commonFindLabel(_pvCtx, _piAddress, _pstLabelName) + 1;

#ifdef DEBUG
    sciprint("DEBUG: addVectorOfIntInPList - itemPos = %d _pstLabelName = %s\n", itemPos, _pstLabelName);
#endif

    tmp_val = (double *)MALLOC(_iNbValues * sizeof(double));
    for (i = 0; i < _iNbValues; i++)
    {
        tmp_val[i] = (double)_piValue[i];
    }
    _SciErr = createMatrixOfDoubleInList(_pvCtx, _iVar, _piAddress, itemPos, _iNbValues, 1, tmp_val);

    if (tmp_val)
    {
        FREE(tmp_val);
        tmp_val = NULL;
    }

    return _SciErr;
}
/*--------------------------------------------------------------------------*/
SciErr createColVectorOfDoubleInPList(void* _pvCtx, int _iVar, int * _piAddress, char * _pstLabelName, int _iNbValues, double * _pdblValue)
{
    int itemPos = -1;
    SciErr _SciErr;

    _SciErr.iErr = 0;
    _SciErr.iMsgCount = 0;

    itemPos = commonFindLabel(_pvCtx, _piAddress, _pstLabelName) + 1;

#ifdef DEBUG
    sciprint("DEBUG: addVectorOfDoubleInPList - itemPos = %d _pstLabelName = %s\n", itemPos, _pstLabelName);
#endif

    _SciErr = createMatrixOfDoubleInList(_pvCtx, _iVar, _piAddress, itemPos, _iNbValues, 1, _pdblValue);

    return _SciErr;
}
/*--------------------------------------------------------------------------*/
/* Utility functions */
static int commonFindLabelPartial(void* _pvCtx, int * _piAddress, char const * const _pstLabelToFind)
{
    int Pos = -1, i = 0;
    int m_label = 0, n_label = 0;
    int * len_label = NULL;
    char ** label_list = NULL;
    SciErr _SciErr;

    _SciErr.iErr = 0;
    _SciErr.iMsgCount = 0;

    _SciErr = getMatrixOfStringInList(_pvCtx, _piAddress, 1, &m_label, &n_label, NULL, NULL);
    len_label = (int *)MALLOC(m_label * n_label * sizeof(int));
    _SciErr = getMatrixOfStringInList(_pvCtx, _piAddress, 1, &m_label, &n_label, len_label, NULL);
    label_list = (char **)MALLOC(m_label * n_label * sizeof(char *));

    for (i = 0;  i < n_label * m_label; i++)
    {
        label_list[i] = (char *)MALLOC((len_label[i] + 1) * sizeof(char));
    }
    _SciErr = getMatrixOfStringInList(_pvCtx, _piAddress, 1, &m_label, &n_label, len_label, label_list);

    if (label_list != NULL)
    {
        for (i = 0; i < m_label * n_label; i++)
        {
            /* A bug in scilab: if the mlist contains only the type, the C API returns m_label*n_label==2 !! */
            if (label_list[i] != NULL)
            {
                if (strncmp(label_list[i], _pstLabelToFind, strlen(_pstLabelToFind)) == 0)
                {
                    Pos = i;

                    if (len_label)
                    {
                        FREE(len_label);
                        len_label = NULL;
                    }
                    freeArrayOfString(label_list, m_label * n_label);

                    return Pos;
                }
            }
        }
    }

    if (len_label)
    {
        FREE(len_label);
        len_label = NULL;
    }
    freeArrayOfString(label_list, m_label * n_label);

    return Pos;
}
/*--------------------------------------------------------------------------*/
static int commonFindLabel(void* _pvCtx, int * _piAddress, char const * const _pstLabelToFind)
{
    int Pos = -1, i = 0;
    int m_label = 0, n_label = 0;
    int * len_label = NULL;
    char ** label_list = NULL;
    SciErr _SciErr;

    _SciErr.iErr = 0;
    _SciErr.iMsgCount = 0;

    _SciErr = getMatrixOfStringInList(_pvCtx, _piAddress, 1, &m_label, &n_label, NULL, NULL);
    len_label = (int *)MALLOC(m_label * n_label * sizeof(int));
    _SciErr = getMatrixOfStringInList(_pvCtx, _piAddress, 1, &m_label, &n_label, len_label, NULL);
    label_list = (char **)MALLOC(m_label * n_label * sizeof(char *));
    for (i = 0; i < n_label * m_label; i++)
    {
        label_list[i] = (char *)MALLOC((len_label[i] + 1) * sizeof(char));
    }
    _SciErr = getMatrixOfStringInList(_pvCtx, _piAddress, 1, &m_label, &n_label, len_label, label_list);

    if (label_list != NULL)
    {
        for (i = 0; i < m_label * n_label; i++)
        {
            /* A bug in scilab: if the mlist contains only the type, the C API returns m_label*n_label==2 !! */
            if (label_list[i] != NULL)
            {
                if (strcmp(label_list[i], (char *)_pstLabelToFind) == 0)
                {
                    Pos = i;

                    if (len_label)
                    {
                        FREE(len_label);
                        len_label = NULL;
                    }
                    freeArrayOfString(label_list, m_label * n_label);

                    return Pos;
                }
            }
        }
    }

    if (len_label)
    {
        FREE(len_label);
        len_label = NULL;
    }
    freeArrayOfString(label_list, m_label * n_label);

    return Pos;
}
/*--------------------------------------------------------------------------*/
// c versions of std::fill_n
void int_fill_n(int* dst, size_t n, int v)
{
    for (; n; --n, ++dst)
    {
        *dst = v;
    }
}
/*--------------------------------------------------------------------------*/
void double_fill_n(double* dst, size_t n, int v)
{
    for (; n; --n, ++dst)
    {
        *dst = v;
    }
}
/*--------------------------------------------------------------------------*/

