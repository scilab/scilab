/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

extern "C"
{
#include "gw_hdf5.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "localization.h"
}

#include "HDF5Scilab.hxx"

using namespace org_modules_hdf5;

template <typename U>
int h5create(char *fname, unsigned long fname_len)
{
    H5Object * hobj = 0;
    U * newobj = 0;
    SciErr err;
    int * addr = 0;
    char * name = 0;
    char * targetType = 0;
    std::string _name;
    std::string _targetType("");
    const hsize_t dims[2] = {0, 0};
    int * col = (int *)&dims[0];
    int * row = (int *)&dims[1];
    int type;
    const int nbIn = nbInputArgument(pvApiCtx);

    CheckOutputArgument(pvApiCtx, 1, 1);
    CheckInputArgument(pvApiCtx, 3, 4);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (HDF5Scilab::isH5Object(addr, pvApiCtx))
    {
        hobj = HDF5Scilab::getH5Object(addr, pvApiCtx);
    }
    else
    {
        Scierror(999, _("%s: Invalid input argument #%d: a H5Object expected.\n"), fname, 1);
    }

    err = getVarAddressFromPosition(pvApiCtx, 2, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    if (!isStringType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
    {
        Scierror(999, gettext("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
        return 0;
    }

    if (getAllocatedSingleString(pvApiCtx, addr, &name) != 0)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    _name = std::string(name);
    freeAllocatedSingleString(name);

    if (nbIn == 4)
    {
        err = getVarAddressFromPosition(pvApiCtx, 4, &addr);
        if (err.iErr)
        {
            printError(&err, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 4);
            return 0;
        }

        if (!isStringType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
        {
            Scierror(999, gettext("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 4);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, addr, &targetType) != 0)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        _targetType = std::string(targetType);
        freeAllocatedSingleString(targetType);
    }

    err = getVarAddressFromPosition(pvApiCtx, 3, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
        return 0;
    }

    err = getVarType(pvApiCtx, addr, &type);
    if (err.iErr)
    {
        Scierror(999, _("%s: Can not get the type of input argument #%d.\n"), fname, 3);
    }

    try
    {
        switch (type)
        {
        case sci_matrix :
        {
            double * mat = 0;

            if (isVarComplex(pvApiCtx, addr))
            {
                Scierror(999, _("%s: Complex datatype is not handled.\n"), fname);
                return 0;
                /*double * imag = 0;
                  err = getComplexMatrixOfDouble(pvApiCtx, addr, row, col, &mat, &imag);
                  if (err.iErr)
                  {

                  }*/
            }
            else
            {
                err = getMatrixOfDouble(pvApiCtx, addr, row, col, &mat);
                if (err.iErr)
                {
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
                    return 0;
                }
                newobj = &HDF5Scilab::create<double, U>(*hobj, _name, 2, dims, 0, mat, _targetType);
            }
            break;
        }
        case sci_ints :
        {
            int prec = 0;
            void * ints = 0;

            err = getMatrixOfIntegerPrecision(pvApiCtx, addr, &prec);
            if (err.iErr)
            {
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
                return 0;
            }

            switch (prec)
            {
            case SCI_INT8 :
                err = getMatrixOfInteger8(pvApiCtx, addr, row, col, (char **)(&ints));
                if (err.iErr)
                {
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
                    return 0;
                }
                newobj = &HDF5Scilab::create<char, U>(*hobj, _name, 2, dims, 0, (char *)ints, _targetType);
                break;
            case SCI_UINT8 :
                err = getMatrixOfUnsignedInteger8(pvApiCtx, addr, row, col, (unsigned char **)(&ints));
                if (err.iErr)
                {
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
                    return 0;
                }
                newobj = &HDF5Scilab::create<unsigned char, U>(*hobj, _name, 2, dims, 0, (unsigned char *)ints, _targetType);
                break;
            case SCI_INT16 :
                err = getMatrixOfInteger16(pvApiCtx, addr, row, col, (short **)(&ints));
                if (err.iErr)
                {
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
                    return 0;
                }
                newobj = &HDF5Scilab::create<short, U>(*hobj, _name, 2, dims, 0, (short *)ints, _targetType);
                break;
            case SCI_UINT16 :
                err = getMatrixOfUnsignedInteger16(pvApiCtx, addr, row, col, (unsigned short **)(&ints));
                if (err.iErr)
                {
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
                    return 0;
                }
                newobj = &HDF5Scilab::create<unsigned short, U>(*hobj, _name, 2, dims, 0, (unsigned short *)ints, _targetType);
                break;
            case SCI_INT32 :
                err = getMatrixOfInteger32(pvApiCtx, addr, row, col, (int**)(&ints));
                if (err.iErr)
                {
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
                    return 0;
                }
                newobj = &HDF5Scilab::create<int, U>(*hobj, _name, 2, dims, 0, (int *)ints, _targetType);
                break;
            case SCI_UINT32 :
                err = getMatrixOfUnsignedInteger32(pvApiCtx, addr, row, col, (unsigned int **)(&ints));
                if (err.iErr)
                {
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
                    return 0;
                }
                newobj = &HDF5Scilab::create<unsigned int, U>(*hobj, _name, 2, dims, 0, (unsigned int *)ints, _targetType);
                break;

#ifdef __SCILAB_INT64__
            case SCI_INT64 :
                err = getMatrixOfInteger64(pvApiCtx, addr, row, col, (long long **)(&ints));
                if (err.iErr)
                {
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
                    return 0;
                }
                newobj = &HDF5Scilab::create<long long, U>(*hobj, _name, 2, dims, 0, (long long *)ints, _targetType);
                break;
            case SCI_UINT64 :
                err = getMatrixOfUnsignedInteger64(pvApiCtx, addr, row, col, (unsigned long long **)(&ints));
                if (err.iErr)
                {
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
                    return 0;
                }
                newobj = &HDF5Scilab::create<unsigned long long, U>(*hobj, _name, 2, dims, 0, (unsigned long long *)ints, _targetType);
                break;
#endif
            }
	    break;
        }
        case sci_strings :
        {
            Scierror(999, _("%s: String datatype is not handled.\n"), fname);
            return 0;
            /*char ** matS = 0;
              if (getAllocatedMatrixOfString(pvApiCtx, addr, row, col, &matS))
              {
              Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
              return 0;
              }*/
        }
        case sci_boolean :
        {
            int * matB;

            err = getMatrixOfBoolean(pvApiCtx, addr, row, col, &matB);
            if (err.iErr)
            {
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
                return 0;
            }
            newobj = &HDF5Scilab::create<int, U>(*hobj, _name, 2, dims, 0, matB, _targetType);
            break;
        }
        default :
        {
            Scierror(999, _("%s: Datatype not handled.\n"), fname);
            return 0;
        }
        }
    }
    catch (const H5Exception & e)
    {
        Scierror(999, _("%s: %s\n"), fname, e.what());
        return 0;
    }

    try
    {
        newobj->createOnScilabStack(nbIn + 1, pvApiCtx);
    }
    catch (const H5Exception & e)
    {
        Scierror(999, _("%s: %s\n"), fname, e.what());
        return 0;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbIn + 1;
    ReturnArguments(pvApiCtx);

    return 0;
}

/*--------------------------------------------------------------------------*/
