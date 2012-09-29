/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __HDF5SCILAB_HXX__
#define __HDF5SCILAB_HXX__

#include <vector>

#include "H5Object.hxx"
#include "H5File.hxx"
#include "H5Group.hxx"
#include "H5Dataset.hxx"
#include "H5Dataspace.hxx"
#include "H5Attribute.hxx"
#include "H5BasicData.hxx"
#include "H5Data.hxx"
#include "H5VariableScope.hxx"

extern "C"
{
#include "api_scilab.h"
#include "Scierror.h"
#include "HDF5Objects.h"
#include "sciprint.h"
#include "localization.h"
}

namespace org_modules_hdf5
{

class HDF5Scilab
{

public:

    enum H5ObjectType {
        H5FILE,
        H5GROUP,
        H5DATASET,
        H5ATTRIBUTE,
        H5SPACE,
        H5TYPE,
        H5REFERENCE,
        H5LIST
    };

    static int getH5ObjectId(int * mlist, void * pvApiCtx);

    static H5Object * getH5Object(int * mlist, void * pvApiCtx);

    static bool isH5Object(int * mlist, void * pvApiCtx);

    static void scilabPrint(const std::string & str);

    static void split(const std::string & str, std::vector<std::string> & v, const char c = '\n');

    static void readData(const std::string & filename, const std::string & name, const unsigned int size, const double * start, const double * stride, const double * count, const double * block, int pos, void * pvApiCtx);

    static void readData(H5Object & obj, const std::string & name, const unsigned int size, const double * start, const double * stride, const double * count, const double * block, int pos, void * pvApiCtx);

    static void readAttributeData(H5Object & obj, const std::string & path, const std::string & attrName, int pos, void * pvApiCtx);

    static void readAttributeData(const std::string & filename, const std::string & path, const std::string & attrName, int pos, void * pvApiCtx);

    static void deleteObject(const std::string & file, const std::string & name);

    static void deleteObject(const std::string & file, const int size, const char ** names);

    static void deleteObject(const H5Object & parent, const std::string & name);

    static void deleteObject(const H5Object & parent, const int size, const char ** names);

    static void getObject(H5Object & parent, const std::string & location, int position, void * pvApiCtx);

    static void createLink(H5Object & parent, const std::string & name, const std::string & targetPath, const bool hard);

    static void createLink(H5Object & parent, const std::string & name, H5Object & targetObject, const bool hard);

    static void createLink(H5Object & parent, const std::string & name, const std::string & targetFile, const std::string & targetPath);

    static void createLink(H5Object & parent, const std::string & name, H5Object & targetObject);

    static void createLink(const std::string & file, const std::string & location, const std::string & name, const std::string & destName, const bool hard);

    static void createLink(const std::string & file, const std::string & location, const std::string & name, const std::string & destFile, const std::string & destName);

    static void copy(H5Object & src, const std::string & slocation, H5Object & dest, const std::string & dlocation);

    static void copy(H5Object & src, const std::string & slocation, const std::string & dfile, const std::string & dlocation);

    static void copy(const std::string & sfile, const std::string & slocation, H5Object & dest, const std::string & dlocation);

    static void copy(const std::string & sfile, const std::string & slocation, const std::string & dfile, const std::string & dlocation);

    static void ls(H5Object & obj, std::string name, int position, void * pvApiCtx);

    static void ls(std::string path, std::string name, int position, void * pvApiCtx);

    static bool checkType(const H5Object & obj, const H5ObjectType type);

    static void mount(H5Object & obj, const std::string & location, H5Object & file);

    static void umount(H5Object & obj, const std::string & location);

    static void createGroup(H5Object & parent, const std::string & name);

    static void createGroup(const std::string & file, const std::string & name);

    static void createGroup(H5Object & parent, const int size, const char ** names);

    static void createGroup(const std::string & file, const int size, const char ** names);

    template <typename T, typename U>
    static U & create(H5Object & parent, const std::string & name, const unsigned int rank, const hsize_t * dims, const hsize_t * maxdims, T * data, const hid_t targetType, const unsigned int size, const double * start, const double * stride, const double * count, const double * block)
    {
        hid_t obj;
        hid_t srcspace;
        hid_t targetspace;
        hid_t type;
        hid_t targettype;
        herr_t err;
        hsize_t * newdims = 0;
        bool mustDelete = false;
        H5T_conv_t conv;
        H5T_cdata_t * pcdata = 0;
        hsize_t * rdims = 0;
        hsize_t * rmaxdims = 0;

        if (rank > __SCILAB_HDF5_MAX_DIMS__)
        {
            throw H5Exception(__LINE__, __FILE__, _("Invalid rank, must be in the interval [0, %d]."), __SCILAB_HDF5_MAX_DIMS__);
        }

        type = H5Type::getBaseType(data);
        if (type < 0)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot create a data type."));
        }

        if (targetType == (hid_t) - 1)
        {
            targettype = type;
        }
        else
        {
            targettype = targetType;
        }

        if (!H5Tfind(type, targettype, &pcdata))
        {
            H5Tclose(type);
            throw H5Exception(__LINE__, __FILE__, _("No converter found for the specified target datatype."));
        }

        rdims = new hsize_t[rank];
        for (unsigned int i = 0; i < rank; i++)
        {
            rdims[i] = dims[rank - 1 - i];
        }

        if (maxdims)
        {
            rmaxdims = new hsize_t[rank];
            for (unsigned int i = 0; i < rank; i++)
            {
                rmaxdims[i] = maxdims[rank - 1 - i];
            }
        }

        srcspace = H5Screate_simple(rank, rdims, rmaxdims);
        delete[] rdims;
        if (rmaxdims)
        {
            delete[] rmaxdims;
        }

        if (srcspace < 0)
        {
            H5Tclose(type);
            throw H5Exception(__LINE__, __FILE__, _("Cannot create a new dataspace."));
        }

        try
        {
            newdims = H5Dataspace::select(srcspace, size, start, stride, count, block);
        }
        catch (const H5Exception & e)
        {
            H5Tclose(type);
            H5Sclose(srcspace);
            throw;
        }

        if (newdims)
        {
            targetspace = H5Screate_simple(rank, newdims, 0);
            delete[] newdims;
            if (targetspace < 0)
            {
                H5Tclose(type);
                H5Sclose(srcspace);
                throw H5Exception(__LINE__, __FILE__, _("Cannot create a new dataspace."));
            }
        }
        else
        {
            targetspace = (hid_t) - 1;
        }

        try
        {
            obj = U::create(parent, name, type, targettype, srcspace, targetspace, data);
            H5Tclose(type);
            H5Sclose(srcspace);
            if (targetspace >= 0)
            {
                H5Sclose(targetspace);
            }
        }
        catch (const H5Exception & e)
        {
            H5Tclose(type);
            H5Sclose(srcspace);
            if (targetspace >= 0)
            {
                H5Sclose(targetspace);
            }
            throw;
        }

        return *new U(parent, obj, name);
    }

    template <typename T, typename U>
    static U & create(H5Object & parent, const std::string & name, const unsigned int rank, const hsize_t * dims, const hsize_t * maxdims, T * data, const std::string & targetType, const unsigned int size, const double * start, const double * stride, const double * count, const double * block)
    {
        hid_t targettype;
        if (targetType.empty())
        {
            targettype = (hid_t) - 1;
        }
        else
        {
            targettype = H5Type::getBaseType(targetType);
            if (targettype < 0)
            {
                throw H5Exception(__LINE__, __FILE__, _("Cannot create the target type."));
            }
        }

        try
        {
            U & obj = create<T, U>(parent, name, rank, dims, maxdims, data, targettype, size, start, stride, count, block);
            if (targettype >= 0)
            {
                H5Tclose(targettype);
            }
            return obj;
        }
        catch (const H5Exception & e)
        {
            if (targettype >= 0)
            {
                H5Tclose(targettype);
            }
            throw;
        }
    }

    template <typename T>
    static void createObjectFromStack(const std::string & file, const std::string & location, const std::string & name, const std::string & targetType, const unsigned int size, const double * start, const double * stride, const double * count, const double * block, const double * maxdims, void * pvApiCtx, int * addr, const int rhsPosition)
    {
        H5File & src = *new H5File(file, "/", "r+");

        try
        {
            createObjectFromStack<T>(src, location, name, targetType, size, start, stride, count, block, maxdims, pvApiCtx, addr, rhsPosition);
            delete &src;
        }
        catch (const H5Exception & e)
        {
            delete &src;
            throw;
        }
    }

    template <typename T>
    static void createObjectFromStack(H5Object & obj, const std::string & location, const std::string & name, const std::string & targetType, const unsigned int size, const double * start, const double * stride, const double * count, const double * block, const double * maxdims, void * pvApiCtx, int * addr, const int rhsPosition)
    {
        T * newobj = 0;
        SciErr err;
        hsize_t dims[2] = { 0, 0 };
        hsize_t * _maxdims = 0;
        hsize_t mdims[2];
        int row;
        int col;
        int type;
        H5Object * hobj = 0;

        if (maxdims)
        {
            mdims[0] = std::numeric_limits<double>::infinity() == maxdims[0] ? H5S_UNLIMITED : (hsize_t)maxdims[0];
            mdims[1] = std::numeric_limits<double>::infinity() == maxdims[1] ? H5S_UNLIMITED : (hsize_t)maxdims[1];
            _maxdims = mdims;
        }

        err = getVarType(pvApiCtx, addr, &type);
        if (err.iErr)
        {
            throw H5Exception(__LINE__, __FILE__, _("Can not get the type of input argument #%d."), rhsPosition);
        }

        hobj = H5Object::isEmptyPath(location) ? &obj : &H5Object::getObject(obj, location);

        try
        {
            switch (type)
            {
                case sci_matrix :
                {
                    double * mat = 0;

                    if (isVarComplex(pvApiCtx, addr))
                    {
                        throw H5Exception(__LINE__, __FILE__, _("Complex datatype not handled for now."));
                    }
                    else
                    {
                        err = getMatrixOfDouble(pvApiCtx, addr, &row, &col, &mat);
                        if (err.iErr)
                        {
                            throw H5Exception(__LINE__, __FILE__, _("%s: Can not read input argument #%d."), rhsPosition);
                        }
                        dims[0] = col;
                        dims[1] = row;
                        newobj = &HDF5Scilab::create<double, T>(*hobj, name, 2, dims, _maxdims, mat, targetType, size, start, stride, count, block);
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
                        throw H5Exception(__LINE__, __FILE__, _("%s: Can not read input argument #%d."), rhsPosition);
                    }

                    switch (prec)
                    {
                        case SCI_INT8 :
                            err = getMatrixOfInteger8(pvApiCtx, addr, &row, &col, (char **)(&ints));
                            if (err.iErr)
                            {
                                throw H5Exception(__LINE__, __FILE__, _("%s: Can not read input argument #%d."), rhsPosition);
                            }
                            dims[0] = col;
                            dims[1] = row;
                            newobj = &HDF5Scilab::create<char, T>(*hobj, name, 2, dims, _maxdims, (char *)ints, targetType, size, start, stride, count, block);
                            break;
                        case SCI_UINT8 :
                            err = getMatrixOfUnsignedInteger8(pvApiCtx, addr, &row, &col, (unsigned char **)(&ints));
                            if (err.iErr)
                            {
                                throw H5Exception(__LINE__, __FILE__, _("%s: Can not read input argument #%d."), rhsPosition);
                            }
                            dims[0] = col;
                            dims[1] = row;
                            newobj = &HDF5Scilab::create<unsigned char, T>(*hobj, name, 2, dims, _maxdims, (unsigned char *)ints, targetType, size, start, stride, count, block);
                            break;
                        case SCI_INT16 :
                            err = getMatrixOfInteger16(pvApiCtx, addr, &row, &col, (short **)(&ints));
                            if (err.iErr)
                            {
                                throw H5Exception(__LINE__, __FILE__, _("%s: Can not read input argument #%d."), rhsPosition);
                            }
                            dims[0] = col;
                            dims[1] = row;
                            newobj = &HDF5Scilab::create<short, T>(*hobj, name, 2, dims, _maxdims, (short *)ints, targetType, size, start, stride, count, block);
                            break;
                        case SCI_UINT16 :
                            err = getMatrixOfUnsignedInteger16(pvApiCtx, addr, &row, &col, (unsigned short **)(&ints));
                            if (err.iErr)
                            {
                                throw H5Exception(__LINE__, __FILE__, _("%s: Can not read input argument #%d."), rhsPosition);
                            }
                            dims[0] = col;
                            dims[1] = row;
                            newobj = &HDF5Scilab::create<unsigned short, T>(*hobj, name, 2, dims, _maxdims, (unsigned short *)ints, targetType, size, start, stride, count, block);
                            break;
                        case SCI_INT32 :
                            err = getMatrixOfInteger32(pvApiCtx, addr, &row, &col, (int**)(&ints));
                            if (err.iErr)
                            {
                                throw H5Exception(__LINE__, __FILE__, _("%s: Can not read input argument #%d."), rhsPosition);
                            }
                            dims[0] = col;
                            dims[1] = row;
                            newobj = &HDF5Scilab::create<int, T>(*hobj, name, 2, dims, _maxdims, (int *)ints, targetType, size, start, stride, count, block);
                            break;
                        case SCI_UINT32 :
                            err = getMatrixOfUnsignedInteger32(pvApiCtx, addr, &row, &col, (unsigned int **)(&ints));
                            if (err.iErr)
                            {
                                throw H5Exception(__LINE__, __FILE__, _("%s: Can not read input argument #%d."), rhsPosition);
                            }
                            dims[0] = col;
                            dims[1] = row;
                            newobj = &HDF5Scilab::create<unsigned int, T>(*hobj, name, 2, dims, _maxdims, (unsigned int *)ints, targetType, size, start, stride, count, block);
                            break;

#ifdef __SCILAB_INT64__
                        case SCI_INT64 :
                            err = getMatrixOfInteger64(pvApiCtx, addr, &row, &col, (long long **)(&ints));
                            if (err.iErr)
                            {
                                throw H5Exception(__LINE__, __FILE__, _("%s: Can not read input argument #%d."), rhsPosition);
                            }
                            dims[0] = col;
                            dims[1] = row;
                            newobj = &HDF5Scilab::create<long long, T>(*hobj, name, 2, dims, _maxdims, (long long *)ints, targetType, size, start, stride, count, block);
                            break;
                        case SCI_UINT64 :
                            err = getMatrixOfUnsignedInteger64(pvApiCtx, addr, &row, &col, (unsigned long long **)(&ints));
                            if (err.iErr)
                            {
                                throw H5Exception(__LINE__, __FILE__, _("%s: Can not read input argument #%d."), rhsPosition);
                            }
                            dims[0] = col;
                            dims[1] = row;
                            newobj = &HDF5Scilab::create<unsigned long long, T>(*hobj, name, 2, dims, _maxdims, (unsigned long long *)ints, targetType, size, start, stride, count, block);
                            break;
#endif
                    }
                    break;
                }
                case sci_strings :
                {
                    char ** matS = 0;
                    if (getAllocatedMatrixOfString(pvApiCtx, addr, &row, &col, &matS))
                    {
                        throw H5Exception(__LINE__, __FILE__, _("%s: Can not read input argument #%d."), rhsPosition);
                    }
                    try
                    {
                        dims[0] = col;
                        dims[1] = row;
                        newobj = &HDF5Scilab::create<char *, T>(*hobj, name, 2, dims, _maxdims, matS, targetType, size, start, stride, count, block);
                    }
                    catch (const H5Exception & e)
                    {
                        freeAllocatedMatrixOfString(*&row, *&col, matS);
                        throw;
                    }

                    freeAllocatedMatrixOfString(*&row, *&col, matS);
                    break;
                }
                case sci_boolean :
                {
                    int * matB;

                    err = getMatrixOfBoolean(pvApiCtx, addr, &row, &col, &matB);
                    if (err.iErr)
                    {
                        throw H5Exception(__LINE__, __FILE__, _("%s: Can not read input argument #%d."), rhsPosition);
                    }
                    dims[0] = col;
                    dims[1] = row;
                    newobj = &HDF5Scilab::create<int, T>(*hobj, name, 2, dims, _maxdims, matB, targetType, size, start, stride, count, block);
                    break;
                }
                default :
                {
                    throw H5Exception(__LINE__, __FILE__, _("%s: Datatype not handled for now."));
                }
            }
        }
        catch (const H5Exception & e)
        {
            if (!H5Object::isEmptyPath(location))
            {
                delete hobj;
            }

            throw;
        }

        if (!H5Object::isEmptyPath(location))
        {
            delete hobj;
        }
    }

};
}

#endif // __HDF5SCILAB_HXX__
