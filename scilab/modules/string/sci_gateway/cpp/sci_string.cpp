/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <math.h>
#include <sstream>
#include "string.hxx"
#include "string_gw.hxx"
#include "tostring_common.hxx"

extern "C"
{
#include "os_wcsdup.h"
#include "Scierror.h"
#include "localization.h"
}

using namespace types;

static void DoubleComplexMatrix2String(wostringstream *_postr,  double _dblR, double _dblI)
{
    /*
    if R && !C -> R
    if R && C -> R + Ci
    if !R && !C -> 0
    if(!R aa C	-> Ci
    */

    if(_dblR == 0)
    {//no real part
        if(_dblI == 0)
        {//no imaginary part

            //0
            *_postr << (_dblI < 0 ? L"-" : L"");
            *_postr << left << 0;
        }
        else
        {//imaginary part

            //I
            *_postr << (_dblI < 0 ? L"-" : L"");
            *_postr << L"%i";
            if(fabs(_dblI) != 1)
            {//specail case if I == 1 write only %i and not %i*1
                *_postr << L"*";
                PrintDoubleVar(_postr, _dblI);
            }
        }
    }
    else
    {//real part
        if(_dblI == 0)
        {//no imaginary part

            //R
            *_postr << (_dblR < 0 ? L"-" : L"");
            PrintDoubleVar(_postr, _dblR);
        }
        else
        {//imaginary part

            //R
            *_postr << (_dblR < 0 ? L"-" : L"");
            PrintDoubleVar(_postr, _dblR);
            //I
            *_postr << (_dblI < 0 ? L"-%i" : L"+%i");
            if(fabs(_dblI) != 1)
            {//special case if I == 1 write only %i and not %i*1
                *_postr << L"*";
                PrintDoubleVar(_postr, _dblI);
            }
        }
    }
}

Function::ReturnValue sci_string(typed_list &in, int _iRetCount, typed_list &out)
{
    if(in.size() != 1)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d expected.\n"), L"string", 1);
        return Function::Error;
    }

    switch(in[0]->getType())
    {
    case GenericType::RealDouble :
        {
            int iRows = in[0]->getAsDouble()->rows_get();
            int iCols = in[0]->getAsDouble()->cols_get();

            // Special case string([]) == []
            if(iRows == 0 && iCols == 0)
            {
                out.push_back(Double::Empty());
                return Function::OK;
            }
            else if(iRows == -1 && iCols == -1)
            {
                out.push_back(new String(1,1));
                return Function::OK;
            }


            String *pstOutput = new String(iRows, iCols);
            for (int i = 0; i < iRows * iCols; ++i)
            {
                std::wostringstream ostr;
                double dblReal = in[0]->getAsDouble()->real_get()[i];
                double dblImg  = 0.0;
                if (in[0]->getAsDouble()->isComplex() == true)
                {
                    dblImg  = in[0]->getAsDouble()->img_get()[i];
                }
                DoubleComplexMatrix2String(&ostr, dblReal, dblImg);
                pstOutput->string_set(i, ostr.str().c_str());
            }
            out.push_back(pstOutput);
            break;
        }
    case GenericType::RealString :
        {
            out.push_back(in[0]);
            break;
        }
    default :
        {
            out.push_back(new types::String((in[0]->toString(0,0)).c_str()));
            break;
        }
    }


    return Function::OK;
}
