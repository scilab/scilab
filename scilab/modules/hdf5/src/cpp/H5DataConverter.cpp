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

#include "H5DataConverter.hxx"

namespace org_modules_hdf5
{

    int * H5DataConverter::getHypermatrix(void * pvApiCtx, const int position, const int ndims, const hsize_t * dims)
    {
        static const char * hypermat[3] = {"hm", "dims", "entries"};

        int * list = 0;
        SciErr err = createMList(pvApiCtx, position, 3, &list);
        if (err.iErr)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot create an hypermatrix on the stack"));
        }

        err = createMatrixOfStringInList(pvApiCtx, position, list, 1, 1, 3, hypermat);
        if (err.iErr)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot create an hypermatrix on the stack"));
        }

	if (sizeof(int) == sizeof(hsize_t))
	{
	    err = createMatrixOfInteger32InList(pvApiCtx, position, list, 2, 1, ndims, (int *)dims);
	    if (err.iErr)
	    {
		throw H5Exception(__LINE__, __FILE__, _("Cannot create an hypermatrix on the stack"));
	    }
	}
	else
	{
	    int * _dims = 0;
	    err = allocMatrixOfInteger32InList(pvApiCtx, position, list, 2, 1, ndims, &_dims);
	    if (err.iErr)
	    {
		throw H5Exception(__LINE__, __FILE__, _("Cannot create an hypermatrix on the stack"));
	    }
	    for (int i = 0; i < ndims; i++)
	    {
		_dims[i] = dims[i];
	    }
	}

        return list;
    }

    void H5DataConverter::toScilabString(void * pvApiCtx, const int position, const int ndims, const hsize_t * dims, char ** data)
    {
	SciErr err;
        if (ndims == 1)
        {
            err = createMatrixOfString(pvApiCtx, position, 1, *dims, data);
            if (err.iErr)
            {
                throw H5Exception(__LINE__, __FILE__, _("Cannot create a double matrix on the stack"));
            }
        }
        else
        {
            if (ndims == 2)
            {
                char ** newData = new char*[dims[0] * dims[1]];
                C2FHypermatrix(2, dims, 0, data, newData);
                err = createMatrixOfString(pvApiCtx, position, dims[0], dims[1], newData);
                delete[] newData;
                if (err.iErr)
                {
                    throw H5Exception(__LINE__, __FILE__, _("Cannot allocate a double matrix on the stack"));
                }
            }
            else
            {
                int * list = getHypermatrix(pvApiCtx, position, ndims, dims);
                hsize_t size = dims[0];
                for (int i = 1; i < ndims; i++)
                {
                    size *= dims[i];
                }
                char ** newData = new char*[size];
                C2FHypermatrix(ndims, dims, size, data, newData);

                err = createMatrixOfStringInList(pvApiCtx, position, list, 3, 1, (int)size, newData);
                delete[] newData;
                if (err.iErr)
                {
                    throw H5Exception(__LINE__, __FILE__, _("Cannot create an hypermatrix on the stack"));
                }
            }
        }
    }

    void H5DataConverter::toScilabDouble(void * pvApiCtx, const int position, const int ndims, const hsize_t * dims, double * data)
    {
	SciErr err;
        if (ndims == 1)
        {
            err = createMatrixOfDouble(pvApiCtx, position, 1, *dims, data);
            if (err.iErr)
            {
                throw H5Exception(__LINE__, __FILE__, _("Cannot create a double matrix on the stack"));
            }
        }
        else
        {
            double * newData = 0;
            if (ndims == 2)
            {
                err = allocMatrixOfDouble(pvApiCtx, position, dims[0], dims[1], &newData);
                if (err.iErr)
                {
                    throw H5Exception(__LINE__, __FILE__, _("Cannot allocate a double matrix on the stack"));
                }
                C2FHypermatrix(2, dims, 0, data, newData);
            }
            else
            {
                int * list = getHypermatrix(pvApiCtx, position, ndims, dims);
                hsize_t size = dims[0];
                for (int i = 1; i < ndims; i++)
                {
                    size *= dims[i];
                }

                err = allocMatrixOfDoubleInList(pvApiCtx, position, list, 3, 1, (int)size, &newData);
                if (err.iErr)
                {
                    throw H5Exception(__LINE__, __FILE__, _("Cannot create an hypermatrix on the stack"));
                }
                C2FHypermatrix(ndims, dims, size, data, newData);
            }
        }
    }

    void H5DataConverter::toScilabInt8(void * pvApiCtx, const int position, const int ndims, const hsize_t * dims, char * data)
    {
	SciErr err;
        if (ndims == 1)
        {
            err = createMatrixOfInteger8(pvApiCtx, position, 1, *dims, data);
            if (err.iErr)
            {
                throw H5Exception(__LINE__, __FILE__, _("Cannot create an int8 matrix on the stack"));
            }
        }
        else
        {
            char * newData = 0;
            if (ndims == 2)
            {

                err = allocMatrixOfInteger8(pvApiCtx, position, dims[0], dims[1], &newData);
                if (err.iErr)
                {
                    throw H5Exception(__LINE__, __FILE__, _("Cannot create an int8 matrix on the stack"));
                }
                C2FHypermatrix(2, dims, 0, data, newData);
            }
            else
            {
                int * list = getHypermatrix(pvApiCtx, position, ndims, dims);
                hsize_t size = dims[0];
                for (int i = 1; i < ndims; i++)
                {
                    size *= dims[i];
                }

                err = allocMatrixOfInteger8InList(pvApiCtx, position, list, 3, 1, (int)size, &newData);
                if (err.iErr)
                {
                    throw H5Exception(__LINE__, __FILE__, _("Cannot create an hypermatrix on the stack"));
                }
                C2FHypermatrix(ndims, dims, size, data, newData);
            }
        }
    }

    void H5DataConverter::toScilabUInt8(void * pvApiCtx, const int position, const int ndims, const hsize_t * dims, unsigned char * data)
    {
 	SciErr err;
	if (ndims == 1)
        {
            err = createMatrixOfUnsignedInteger8(pvApiCtx, position, 1, *dims, data);
            if (err.iErr)
            {
                throw H5Exception(__LINE__, __FILE__, _("Cannot create an uint8 matrix on the stack"));
            }
        }
        else
        {
            unsigned char * newData = 0;
            if (ndims == 2)
            {
                err = allocMatrixOfUnsignedInteger8(pvApiCtx, position, dims[0], dims[1], &newData);
                if (err.iErr)
                {
                    throw H5Exception(__LINE__, __FILE__, _("Cannot create an uint8 matrix on the stack"));
                }
                C2FHypermatrix(2, dims, 0, data, newData);
            }
            else
            {
                int * list = getHypermatrix(pvApiCtx, position, ndims, dims);
                hsize_t size = dims[0];
                for (int i = 1; i < ndims; i++)
                {
                    size *= dims[i];
                }

                err = allocMatrixOfUnsignedInteger8InList(pvApiCtx, position, list, 3, 1, (int)size, &newData);
                if (err.iErr)
                {
                    throw H5Exception(__LINE__, __FILE__, _("Cannot create an hypermatrix on the stack"));
                }
                C2FHypermatrix(ndims, dims, size, data, newData);
            }
        }
    }

    void H5DataConverter::toScilabInt16(void * pvApiCtx, const int position, const int ndims, const hsize_t * dims, short * data)
    {
	SciErr err;
        if (ndims == 1)
        {
            err = createMatrixOfInteger16(pvApiCtx, position, 1, *dims, data);
            if (err.iErr)
            {
                throw H5Exception(__LINE__, __FILE__, _("Cannot create an int16 matrix on the stack"));
            }
        }
        else
        {
            short * newData = 0;
            if (ndims == 2)
            {
                err = allocMatrixOfInteger16(pvApiCtx, position, dims[0], dims[1], &newData);
                if (err.iErr)
                {
                    throw H5Exception(__LINE__, __FILE__, _("Cannot create an int16 matrix on the stack"));
                }
                C2FHypermatrix(2, dims, 0, data, newData);
            }
            else
            {
                int * list = getHypermatrix(pvApiCtx, position, ndims, dims);
                hsize_t size = dims[0];
                for (int i = 1; i < ndims; i++)
                {
                    size *= dims[i];
                }

                err = allocMatrixOfInteger16InList(pvApiCtx, position, list, 3, 1, (int)size, &newData);
                if (err.iErr)
                {
                    throw H5Exception(__LINE__, __FILE__, _("Cannot create an hypermatrix on the stack"));
                }
                C2FHypermatrix(ndims, dims, size, data, newData);
            }
        }
    }

    void H5DataConverter::toScilabUInt16(void * pvApiCtx, const int position, const int ndims, const hsize_t * dims, unsigned short * data)
    {
	SciErr err;
        if (ndims == 1)
        {
            err = createMatrixOfUnsignedInteger16(pvApiCtx, position, 1, *dims, data);
            if (err.iErr)
            {
                throw H5Exception(__LINE__, __FILE__, _("Cannot create an uint16 matrix on the stack"));
            }
        }
        else
        {
            unsigned short * newData = 0;
            if (ndims == 2)
            {
                err = allocMatrixOfUnsignedInteger16(pvApiCtx, position, dims[0], dims[1], &newData);
                if (err.iErr)
                {
                    throw H5Exception(__LINE__, __FILE__, _("Cannot create an uint16 matrix on the stack"));
                }
                C2FHypermatrix(2, dims, 0, data, newData);
            }
            else
            {
                int * list = getHypermatrix(pvApiCtx, position, ndims, dims);
                hsize_t size = dims[0];
                for (int i = 1; i < ndims; i++)
                {
                    size *= dims[i];
                }

                err = allocMatrixOfUnsignedInteger16InList(pvApiCtx, position, list, 3, 1, (int)size, &newData);
                if (err.iErr)
                {
                    throw H5Exception(__LINE__, __FILE__, _("Cannot create an hypermatrix on the stack"));
                }
                C2FHypermatrix(ndims, dims, size, data, newData);
            }
        }
    }

    void H5DataConverter::toScilabInt32(void * pvApiCtx, const int position, const int ndims, const hsize_t * dims, int * data)
    {
	SciErr err;
        if (ndims == 1)
        {
            err = createMatrixOfInteger32(pvApiCtx, position, 1, *dims, data);
            if (err.iErr)
            {
                throw H5Exception(__LINE__, __FILE__, _("Cannot create an int32 matrix on the stack"));
            }
        }
        else
        {
            int * newData = 0;
            if (ndims == 2)
            {
                err = allocMatrixOfInteger32(pvApiCtx, position, dims[0], dims[1], &newData);
                if (err.iErr)
                {
                    throw H5Exception(__LINE__, __FILE__, _("Cannot create an int32 matrix on the stack"));
                }
                C2FHypermatrix(2, dims, 0, data, newData);
            }
            else
            {
                int * list = getHypermatrix(pvApiCtx, position, ndims, dims);
                hsize_t size = dims[0];
                for (int i = 1; i < ndims; i++)
                {
                    size *= dims[i];
                }

                err = allocMatrixOfInteger32InList(pvApiCtx, position, list, 3, 1, (int)size, &newData);
                if (err.iErr)
                {
                    throw H5Exception(__LINE__, __FILE__, _("Cannot create an hypermatrix on the stack"));
                }
                C2FHypermatrix(ndims, dims, size, data, newData);
            }
        }
    }

    void H5DataConverter::toScilabUInt32(void * pvApiCtx, const int position, const int ndims, const hsize_t * dims, unsigned int * data)
    {
	SciErr err;
        if (ndims == 1)
        {
            err = createMatrixOfUnsignedInteger32(pvApiCtx, position, 1, *dims, data);
            if (err.iErr)
            {
                throw H5Exception(__LINE__, __FILE__, _("Cannot create an uint32 matrix on the stack"));
            }
        }
        else
        {
            unsigned int * newData = 0;
            if (ndims == 2)
            {
                err = allocMatrixOfUnsignedInteger32(pvApiCtx, position, dims[0], dims[1], &newData);
                if (err.iErr)
                {
                    throw H5Exception(__LINE__, __FILE__, _("Cannot create an uint32 matrix on the stack"));
                }
                C2FHypermatrix(2, dims, 0, data, newData);
            }
            else
            {
                int * list = getHypermatrix(pvApiCtx, position, ndims, dims);
                hsize_t size = dims[0];
                for (int i = 1; i < ndims; i++)
                {
                    size *= dims[i];
                }

                err = allocMatrixOfUnsignedInteger32InList(pvApiCtx, position, list, 3, 1, (int)size, &newData);
                if (err.iErr)
                {
                    throw H5Exception(__LINE__, __FILE__, _("Cannot create an hypermatrix on the stack"));
                }
                C2FHypermatrix(ndims, dims, size, data, newData);
            }
        }
    }

#ifdef __SCILAB_INT64__

    void H5DataConverter::toScilabInt64(void * pvApiCtx, const int position, const int ndims, const hsize_t * dims, long long * data)
    {
	SciErr err;
        if (ndims == 1)
        {
            err = createMatrixOfInteger64(pvApiCtx, position, 1, *dims, data);
            if (err.iErr)
            {
                throw H5Exception(__LINE__, __FILE__, _("Cannot create an int64 matrix on the stack"));
            }
        }
        else
        {
            long long * newData = 0;
            if (ndims == 2)
            {
                err = allocMatrixOfInteger64(pvApiCtx, position, dims[0], dims[1], &newData);
                if (err.iErr)
                {
                    throw H5Exception(__LINE__, __FILE__, _("Cannot create an int64 matrix on the stack"));
                }
                C2FHypermatrix(2, dims, 0, data, newData);
            }
            else
            {
                int * list = getHypermatrix(pvApiCtx, position, ndims, dims);
                hsize_t size = dims[0];
                for (int i = 1; i < ndims; i++)
                {
                    size *= dims[i];
                }

                err = allocMatrixOfInteger64InList(pvApiCtx, position, list, 3, 1, (int)size, &newData);
                if (err.iErr)
                {
                    throw H5Exception(__LINE__, __FILE__, _("Cannot create an hypermatrix on the stack"));
                }
                C2FHypermatrix(ndims, dims, size, data, newData);
            }
        }
    }

    void H5DataConverter::toScilabUInt64(void * pvApiCtx, const int position, const int ndims, const hsize_t * dims, unsigned long long * data)
    {
	SciErr err;
        if (ndims == 1)
        {
            err = createMatrixOfUnsignedInteger64(pvApiCtx, position, 1, *dims, data);
            if (err.iErr)
            {
                throw H5Exception(__LINE__, __FILE__, _("Cannot create an uint64 matrix on the stack"));
            }
        }
        else
        {
            unsigned long long * newData = 0;
            if (ndims == 2)
            {
                err = allocMatrixOfUnsignedInteger64(pvApiCtx, position, dims[0], dims[1], &newData);
                if (err.iErr)
                {
                    throw H5Exception(__LINE__, __FILE__, _("Cannot create an uint64 matrix on the stack"));
                }
                C2FHypermatrix(2, dims, 0, data, newData);
            }
            else
            {
                int * list = getHypermatrix(pvApiCtx, position, ndims, dims);
                hsize_t size = dims[0];
                for (int i = 1; i < ndims; i++)
                {
                    size *= dims[i];
                }

                err = allocMatrixOfUnsignedInteger64InList(pvApiCtx, position, list, 3, 1, (int)size, &newData);
                if (err.iErr)
                {
                    throw H5Exception(__LINE__, __FILE__, _("Cannot create an hypermatrix on the stack"));
                }
                C2FHypermatrix(ndims, dims, size, data, newData);
            }
        }
    }

#endif // __SCILAB_INT64__

}
