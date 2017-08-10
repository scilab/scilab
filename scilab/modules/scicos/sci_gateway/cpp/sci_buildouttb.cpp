/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
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

#include <cstring>
#include <string>
#include <algorithm>

#include "gw_scicos.hxx"

#include "internal.hxx"
#include "types.hxx"
#include "double.hxx"
#include "function.hxx"
#include "int.hxx"
#include "list.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
}

/*--------------------------------------------------------------------------*/
/* intbuildouttb build an initialized outtb list
*
* [outtb]=buildouttb(lnksz,lnktyp)
*
* rhs 1 : lnksz, give the size of scilab object in outtb
* can be all int type or double matrix
* can have n,2 or 2,n size
*
* rhs 2 : lnktyp, gives the type of scilab object in outtb
* 1 : double
* 2 : complex
* 3 : int32
* 4 : int16
* 5 : int8
* 6 : uint32
* 7 : uint16
* 8 : uint8
* else : double
* can be all int type or double matrix
* can have n,1 or 1,n size
*
* lhs 1 : a list of size n
*
* 02/07/06, Alan : Initial version.
* 21/11/06, Alan : Allow void rhs input param.
* 05/12/14, Paul : Rewrite to C++.
*
*/
/*--------------------------------------------------------------------------*/

static const std::string funname = "buildouttb";

types::Function::ReturnValue sci_buildouttb(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), funname.data(), 2);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), funname.data(), 1);
        return types::Function::Error;
    }

    int m1, n1, mn1;
    int* p1Copy;
    int m2, n2, mn2;
    int* p2Copy;

    switch (in[0]->getType())
    {
        case types::InternalType::ScilabDouble:
        {
            types::Double* p1D = in[0]->getAs<types::Double>();
            if (p1D->isComplex())
            {
                Scierror(999, _("%s: Wrong type for input argument #%d : A real or integer matrix expected.\n"), funname.data(), 1);
                return types::Function::Error;
            }
            m1 = p1D->getRows();
            n1 = p1D->getCols();
            mn1 = m1 * n1;

            p1Copy = new int[mn1];
            for (int i = 0; i < mn1; ++i)
            {
                p1Copy[i] = static_cast<int>(p1D->get(i));
            }
            break;
        }
        case types::InternalType::ScilabInt8:
        {
            types::Int8* p1Int8 = in[0]->getAs<types::Int8>();
            m1 = p1Int8->getRows();
            n1 = p1Int8->getCols();
            mn1 = m1 * n1;

            char* buffer = new char[mn1];
            memcpy(buffer, p1Int8->get(), mn1 * sizeof(char));
            p1Copy = new int[mn1];
            memcpy(p1Copy, buffer, mn1 * sizeof(int));
            delete[] buffer;
            break;
        }
        case types::InternalType::ScilabInt16:
        {
            types::Int16* p1Int16 = in[0]->getAs<types::Int16>();
            m1 = p1Int16->getRows();
            n1 = p1Int16->getCols();
            mn1 = m1 * n1;

            char* buffer = new char[mn1];
            memcpy(buffer, p1Int16->get(), mn1 * sizeof(char));
            p1Copy = new int[mn1];
            memcpy(p1Copy, buffer, mn1 * sizeof(int));
            delete[] buffer;
            break;
        }
        case types::InternalType::ScilabInt32:
        {
            types::Int32* p1Int32 = in[0]->getAs<types::Int32>();
            m1 = p1Int32->getRows();
            n1 = p1Int32->getCols();
            mn1 = m1 * n1;

            char* buffer = new char[mn1];
            memcpy(buffer, p1Int32->get(), mn1 * sizeof(char));
            p1Copy = new int[mn1];
            memcpy(p1Copy, buffer, mn1 * sizeof(int));
            delete[] buffer;
            break;
        }
        case types::InternalType::ScilabUInt8:
        {
            types::UInt8* p1UInt8 = in[0]->getAs<types::UInt8>();
            m1 = p1UInt8->getRows();
            n1 = p1UInt8->getCols();
            mn1 = m1 * n1;

            char* buffer = new char[mn1];
            memcpy(buffer, p1UInt8->get(), mn1 * sizeof(char));
            p1Copy = new int[mn1];
            memcpy(p1Copy, buffer, mn1 * sizeof(int));
            delete[] buffer;
            break;
        }
        case types::InternalType::ScilabUInt16:
        {
            types::UInt16* p1UInt16 = in[0]->getAs<types::UInt16>();
            m1 = p1UInt16->getRows();
            n1 = p1UInt16->getCols();
            mn1 = m1 * n1;

            char* buffer = new char[mn1];
            memcpy(buffer, p1UInt16->get(), mn1 * sizeof(char));
            p1Copy = new int[mn1];
            memcpy(p1Copy, buffer, mn1 * sizeof(int));
            delete[] buffer;
            break;
        }
        case types::InternalType::ScilabUInt32:
        {
            types::UInt32* p1UInt32 = in[0]->getAs<types::UInt32>();
            m1 = p1UInt32->getRows();
            n1 = p1UInt32->getCols();
            mn1 = m1 * n1;

            char* buffer = new char[mn1];
            memcpy(buffer, p1UInt32->get(), mn1 * sizeof(char));
            p1Copy = new int[mn1];
            memcpy(p1Copy, buffer, mn1 * sizeof(int));
            delete[] buffer;
            break;
        }
        default:
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A real or integer matrix expected.\n"), funname.data(), 1);
            return types::Function::Error;
        }
    }

    switch (in[1]->getType())
    {
        case types::InternalType::ScilabDouble:
        {
            types::Double* p2D = in[1]->getAs<types::Double>();
            if (p2D->isComplex())
            {
                Scierror(999, _("%s: Wrong type for input argument #%d : A real or integer matrix expected.\n"), funname.data(), 2);
                delete[] p1Copy;
                return types::Function::Error;
            }
            m2 = p2D->getRows();
            n2 = p2D->getCols();
            mn2 = m2 * n2;

            p2Copy = new int[mn2];
            for (int i = 0; i < mn2; ++i)
            {
                p2Copy[i] = static_cast<int>(p2D->get(i));
            }
            break;
        }
        case types::InternalType::ScilabInt8:
        {
            types::Int8* p2Int8 = in[1]->getAs<types::Int8>();
            m2 = p2Int8->getRows();
            n2 = p2Int8->getCols();
            mn2 = m2 * n2;

            char* buffer = new char[mn2];
            memcpy(buffer, p2Int8->get(), mn2 * sizeof(char));
            p2Copy = new int[mn2];
            memcpy(p2Copy, buffer, mn2 * sizeof(int));
            delete[] buffer;
            break;
        }
        case types::InternalType::ScilabInt16:
        {
            types::Int16* p2Int16 = in[1]->getAs<types::Int16>();
            m2 = p2Int16->getRows();
            n2 = p2Int16->getCols();
            mn2 = m2 * n2;

            short* buffer = new short[mn2];
            memcpy(buffer, p2Int16->get(), mn2 * sizeof(short));
            p2Copy = new int[mn2];
            memcpy(p2Copy, buffer, mn2 * sizeof(int));
            delete[] buffer;
            break;
        }
        case types::InternalType::ScilabInt32:
        {
            types::Int32* p2Int32 = in[1]->getAs<types::Int32>();
            m2 = p2Int32->getRows();
            n2 = p2Int32->getCols();
            mn2 = m2 * n2;

            p2Copy = new int[mn2];
            memcpy(p2Copy, p2Int32->get(), mn2 * sizeof(int));
            break;
        }
        case types::InternalType::ScilabUInt8:
        {
            types::UInt8* p2UInt8 = in[1]->getAs<types::UInt8>();
            m2 = p2UInt8->getRows();
            n2 = p2UInt8->getCols();
            mn2 = m2 * n2;

            unsigned char* buffer = new unsigned char[mn2];
            memcpy(buffer, p2UInt8->get(), mn2 * sizeof(unsigned char));
            p2Copy = new int[mn2];
            memcpy(p2Copy, buffer, mn2 * sizeof(int));
            delete[] buffer;
            break;
        }
        case types::InternalType::ScilabUInt16:
        {
            types::UInt16* p2UInt16 = in[1]->getAs<types::UInt16>();
            m2 = p2UInt16->getRows();
            n2 = p2UInt16->getCols();
            mn2 = m2 * n2;

            unsigned short* buffer = new unsigned short[mn2];
            memcpy(buffer, p2UInt16->get(), mn2 * sizeof(unsigned short));
            p2Copy = new int[mn2];
            memcpy(p2Copy, buffer, mn2 * sizeof(int));
            delete[] buffer;
            break;
        }
        case types::InternalType::ScilabUInt32:
        {
            types::UInt32* p2UInt32 = in[1]->getAs<types::UInt32>();
            m2 = p2UInt32->getRows();
            n2 = p2UInt32->getCols();
            mn2 = m2 * n2;

            p2Copy = new int[mn2];
            memcpy(p2Copy, p2UInt32->get(), mn2 * sizeof(int));
            break;
        }
        default:
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A real or integer matrix expected.\n"), funname.data(), 2);
            delete[] p1Copy;
            return types::Function::Error;
        }
    }

    types::List* pOut = new types::List();

    // Check size of Rhs 1
    int np1 = 0;
    if (m1 == 2)
    {
        np1 = n1;
    }
    else if (n1 == 2)
    {
        np1 = m1;
    }
    // void double input give void list output
    else if ((n1 == 0) || (m1 == 0))
    {
        if ((n2 == 0) || (m2 == 0))
        {
            out.push_back(pOut);
            delete[] p1Copy;
            delete[] p2Copy;
            return types::Function::OK;
        }
        else
        {
            Scierror(888, _("%s : inconsistent dimensions between arguments.\n"), funname.data());
            delete[] p1Copy;
            delete[] p2Copy;
            delete pOut;
            return types::Function::Error;
        }
    }
    else
    {
        Scierror(888, _("%s : bad dimension for argument #%d.\n"), funname.data(), 1);
        delete[] p1Copy;
        delete[] p2Copy;
        delete pOut;
        return types::Function::Error;
    }

    // Check size of Rhs 2
    int np2 = 0;
    if (m2 == 1)
    {
        np2 = n2;
    }
    else if (n2 == 1)
    {
        np2 = m2;
    }
    else if ((n2 == 0) || (m2 == 0))
    {
        if ( (n1 != 0) && (m1 != 0) )
        {
            Scierror(888, _("%s : inconsistent dimensions between arguments.\n"), funname.data());
            delete[] p1Copy;
            delete[] p2Copy;
            delete pOut;
            return types::Function::Error;
        }
    }
    else
    {
        Scierror(888, _("%s : bad dimension for argument #%d.\n"), funname.data(), 2);
        delete[] p1Copy;
        delete[] p2Copy;
        delete pOut;
        return types::Function::Error;
    }

    // Cross size checking
    if (np1 != np2)
    {
        Scierror(888, _("%s : arguments must have the same length.\n"), funname.data());
        delete[] p1Copy;
        delete[] p2Copy;
        delete pOut;
        return types::Function::Error;
    }

    for (int i = 0; i < np2; ++i)
    {
        int nm = p1Copy[i] * p1Copy[i + np2];
        switch (p2Copy[i])
        {
            case 1  :
            {
                double* data;
                types::Double* pOutD = new types::Double(p1Copy[i], p1Copy[i + np2], &data);
                for (int j = 0; j < nm; ++j)
                {
                    data[j] = 0;
                }
                pOut->set(i, pOutD);
                break;
            }
            case 2  :
            {
                double* data;
                double* dataComplex;
                types::Double* pOutDC = new types::Double(p1Copy[i], p1Copy[i + np2], &data, &dataComplex);
                for (int j = 0; j < nm; ++j)
                {
                    data[j] = 0;
                    dataComplex[j] = 0;
                }
                pOut->set(i, pOutDC);
                break;
            }
            case 3  :
            {
                int* data;
                types::Int32* pOutInt32 = new types::Int32(p1Copy[i], p1Copy[i + np2], &data);
                for (int j = 0; j < nm; ++j)
                {
                    data[j] = 0;
                }
                pOut->set(i, pOutInt32);
                break;
            }
            case 4  :
            {
                short* data;
                types::Int16* pOutInt16 = new types::Int16(p1Copy[i], p1Copy[i + np2], &data);
                for (int j = 0; j < nm; ++j)
                {
                    data[j] = 0;
                }
                pOut->set(i, pOutInt16);
                break;
            }
            case 5  :
            {
                char* data;
                types::Int8* pOutInt8 = new types::Int8(p1Copy[i], p1Copy[i + np2], &data);
                for (int j = 0; j < nm; ++j)
                {
                    data[j] = 0;
                }
                pOut->set(i, pOutInt8);
                break;
            }
            case 6  :
            {
                unsigned int* data;
                types::UInt32* pOutUInt32 = new types::UInt32(p1Copy[i], p1Copy[i + np2], &data);
                for (int j = 0; j < nm; ++j)
                {
                    data[j] = 0;
                }
                pOut->set(i, pOutUInt32);
                break;
            }
            case 7  :
            {
                unsigned short* data;
                types::UInt16* pOutUInt16 = new types::UInt16(p1Copy[i], p1Copy[i + np2], &data);
                for (int j = 0; j < nm; ++j)
                {
                    data[j] = 0;
                }
                pOut->set(i, pOutUInt16);
                break;
            }
            case 8  :
            {
                unsigned char* data;
                types::UInt8* pOutUInt8 = new types::UInt8(p1Copy[i], p1Copy[i + np2], &data);
                for (int j = 0; j < nm; ++j)
                {
                    data[j] = 0;
                }
                pOut->set(i, pOutUInt8);
                break;
            }
            default:
            {
                // By default, apply case 1
                double* data;
                types::Double* pOutD = new types::Double(p1Copy[i], p1Copy[i + np2], &data);
                for (int j = 0; j < nm; ++j)
                {
                    data[j] = 0;
                }
                pOut->set(i, pOutD);
                break;
            }
        }
    }

    out.push_back(pOut);

    delete[] p1Copy;
    delete[] p2Copy;

    return types::Function::OK;
}
