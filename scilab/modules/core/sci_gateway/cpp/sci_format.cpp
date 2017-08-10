/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008-2011 - DIGITEO - Allan CORNET
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
#include "core_gw.hxx"
#include "function.hxx"
#include "configvariable.hxx"
#include "double.hxx"
#include "string.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
}

/*--------------------------------------------------------------------------*/
#define e_type_format L"e"
#define v_type_format L"v"
#define _e_type_format "e"
#define _v_type_format "v"
#define format_MIN 2
#define format_MAX 25
#define format_e_MIN 8
#define mode_variable 1
#define mode_e 0

types::Function::ReturnValue sci_format(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iType = -1;
    int iLen = -1;

    if (in.size() > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected."), "format", 0, 2);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "format", 0, 1);
        return types::Function::Error;
    }

    if (in.size() == 0)
    {
        //get
        types::Double* pD = new types::Double(1, 2);
        pD->set(0, ConfigVariable::getFormatMode());
        pD->set(1, ConfigVariable::getFormatSize());
        out.push_back(pD);
        return types::Function::OK;
    }
    else if (in.size() == 1 && in[0]->isString())
    {
        //'e' or 'v'
        types::String *pS = in[0]->getAs<types::String>();
        if (pS->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), "format", 1, _e_type_format, _v_type_format);
            return types::Function::Error;
        }

        wchar_t* pwst = pS->get(0);
        if (wcscmp(pwst, e_type_format) == 0)
        {
            iType = 0;
        }
        else if (wcscmp(pwst, v_type_format) == 0)
        {
            iType = 1;
        }
        else
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), "format", 1, _e_type_format, _v_type_format);
            return types::Function::Error;
        }
    }
    else if (in.size() == 1 && in[0]->isDouble())
    {
        types::Double* pD = in[0]->getAs<types::Double>();

        switch (pD->getSize())
        {
            case 2:
            {
                double dType = pD->get(1);
                iType = (int)pD->get(1);

                if (dType != (double)iType)
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: An integer value expected.\n"), "format", 1);
                    return types::Function::Error;
                }

                if (iType != 0 && iType != 1)
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d.\n"), "format", 1);
                    return types::Function::Error;
                }
            }
            case 1:
            {
                double dLen = pD->get(0);
                iLen = (int)pD->get(0);

                if (dLen != (double)iLen)
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: An integer value expected.\n"), "format", 1);
                    return types::Function::Error;
                }

                if (iLen < 0)
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: A positive value expected.\n"), "format", 1);
                    return types::Function::Error;
                }
                break;
            }
            default:
            {
                Scierror(999, _("%s: Wrong size for input argument #%d.\n"), "format", 1);
                return types::Function::Error;
            }
        }
    }
    else if (in.size() == 2)
    {
        //first argument
        if (in[0]->isString() == false && in[0]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected."), "format", 1);
            return types::Function::Error;
        }

        if (in[0]->isString())
        {
            types::String *pS = in[0]->getAs<types::String>();
            if (pS->isScalar() == false)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), "format", 1, _e_type_format, _v_type_format);
                return types::Function::Error;
            }

            wchar_t* pwst = pS->get(0);
            if (wcscmp(pwst, e_type_format) == 0)
            {
                iType = 0;
            }
            else if (wcscmp(pwst, v_type_format) == 0)
            {
                iType = 1;
            }
            else
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), "format", 1, _e_type_format, _v_type_format);
                return types::Function::Error;
            }


            //second argument
            if (in[1]->isDouble() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: An integer expected."), "format", 2);
                return types::Function::Error;
            }

            types::Double* pD = in[1]->getAs<types::Double>();
            if (pD->isScalar() == false)
            {
                //set length
                Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), "format", 2);
                return types::Function::Error;
            }

            double dLen = pD->get(0);
            iLen = (int)pD->get(0);

            if (dLen != (double)iLen)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: An integer value expected.\n"), "format", 2);
                return types::Function::Error;
            }

            if (iLen < 0)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: A positive value expected.\n"), "format", 1);
                return types::Function::Error;
            }
        }
        else //in[0]->isDouble() == true
        {
            types::Double* pD = in[0]->getAs<types::Double>();
            if (pD->isScalar() == false)
            {
                //set length
                Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), "format", 1);
                return types::Function::Error;
            }

            double dLen = pD->get(0);
            iLen = (int)pD->get(0);

            if (dLen != (double)iLen)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: An integer value expected.\n"), "format", 1);
                return types::Function::Error;
            }

            if (iLen < 0)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: A positive value expected.\n"), "format", 1);
                return types::Function::Error;
            }

            if (in[1]->isString() == true)
            {
                types::String *pS = in[1]->getAs<types::String>();
                if (pS->isScalar() == false)
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), "format", 2, _e_type_format, _v_type_format);
                    return types::Function::Error;
                }

                wchar_t* pwst = pS->get(0);
                if (wcscmp(pwst, e_type_format) == 0)
                {
                    iType = 0;
                }
                else if (wcscmp(pwst, v_type_format) == 0)
                {
                    iType = 1;
                }
                else
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), "format", 2, _e_type_format, _v_type_format);
                    return types::Function::Error;
                }
            }
            else if (in[1]->isDouble() == true)
            {
                types::Double* pD = in[1]->getAs<types::Double>();
                if (pD->isScalar() == false)
                {
                    //set length
                    Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), "format", 2);
                    return types::Function::Error;
                }

                double dType = pD->get(0);
                iType = (int)pD->get(0);

                if (dType != (double)iType)
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: An integer value expected.\n"), "format", 2);
                    return types::Function::Error;
                }

                if (iType != 0 && iType != 1)
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d.\n"), "format", 2);
                    return types::Function::Error;
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: a String or Integer expected.\n"), "format", 2);
                return types::Function::Error;
            }
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: a String or Integer expected.\n"), "format", 1);
        return types::Function::Error;
    }

    if (iType == -1)
    {
        //get current type
        iType = ConfigVariable::getFormatMode();
    }

    if (iLen == -1)
    {
        iLen = ConfigVariable::getFormatSize();
        ConfigVariable::setFormatMode(iType);
        if (iType == 1)
        {
            if (iLen < format_MIN)
            {
                ConfigVariable::setFormatSize(format_MIN);
            }
            else if (iLen > format_MAX)
            {
                ConfigVariable::setFormatSize(format_MAX);
            }
        }
        else
        {
            if (iLen < format_e_MIN)
            {
                ConfigVariable::setFormatSize(format_e_MIN);
            }
            else if (iLen > format_MAX)
            {
                ConfigVariable::setFormatSize(format_MAX);
            }
        }
    }

    if (iType == 1)
    {
        if (iLen < format_MIN || iLen > format_MAX)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the interval [%d, %d].\n"), "format", 1, format_MIN, format_MAX);
            return types::Function::Error;
        }

        ConfigVariable::setFormatSize(iLen);
        ConfigVariable::setFormatMode(iType);
    }
    else
    {
        //iType == 0
        if (iLen < format_e_MIN || iLen > format_MAX)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the interval [%d, %d].\n"), "format", 1, format_e_MIN, format_MAX);
            return types::Function::Error;
        }

        ConfigVariable::setFormatSize(iLen);
        ConfigVariable::setFormatMode(iType);
    }

    return types::Function::OK;
}
