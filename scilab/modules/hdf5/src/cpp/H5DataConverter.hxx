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

#ifndef __H5DATACONVERTER_HXX__
#define __H5DATACONVERTER_HXX__

#include <hdf5.h>
#include "H5Exception.hxx"
#include "H5Object.hxx"
#include "H5Data.hxx"

extern "C"
{
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
}

namespace org_modules_hdf5
{

    class H5DataConverter
    {

    public:

        template <typename T>
        static std::string dump(const unsigned int indentLevel, const int ndims, const hsize_t * dims, T * data, const H5Data & obj)
            {
                std::ostringstream os;
                std::string indent = H5Object::getIndentString(indentLevel);

                os << indent << "DATA {" << std::endl;
		printData(indentLevel, indent + "(", os, ndims, dims, &data, obj);
                os << indent << "}" << std::endl;

                return os.str();
            }

        template <typename T>
        static void printData(const unsigned int indentLevel, const std::string & start, std::ostringstream & os, const int ndims, const hsize_t * dims, T ** data, const H5Data & obj)
            {
                std::string indent = H5Object::getIndentString(indentLevel);

		if (ndims == 0)
		{
		    os << start << "0): 0";
		}
                else if (ndims == 1)
                {
                    os << start << "0): ";
                    for (hsize_t i = 0; i < dims[0] - 1; i++)
                    {
			printOneData(os, (*data)[i], obj);
                        os << ", ";
                    }
                    printOneData(os, (*data)[dims[0] - 1], obj);
                    os << std::endl;
                    *data += dims[0];
                }
                else
                {
                    std::ostringstream oss;
                    for (hsize_t i = 0; i < dims[0]; i++)
                    {
                        oss << start << (unsigned int)i << ",";
                        printData(0, oss.str(), os, ndims - 1, dims + 1, data, obj);
                        oss.str("");
                    }
                }
            }

        template <typename T>
        static inline void printOneData(std::ostream & os, T data, const H5Data & obj)
            {
                os << data;
            }

        static inline void printOneData(std::ostream & os, char * data, const H5Data & obj)
            {
                os << "\"" << data << "\"";
            }

	static inline void printOneData(std::ostream & os, void * data, const H5Data & obj)
            {
		obj.printData(os, data);
            }

        static void toScilabString(void * pvApiCtx, const int position, const int ndims, const hsize_t * dims, char ** data);

        static void toScilabDouble(void * pvApiCtx, const int position, const int ndims, const hsize_t * dims, double * data);

        static void toScilabInt8(void * pvApiCtx, const int position, const int ndims, const hsize_t * dims, char * data);

        static void toScilabUInt8(void * pvApiCtx, const int position, const int ndims, const hsize_t * dims, unsigned char * data);

        static void toScilabInt16(void * pvApiCtx, const int position, const int ndims, const hsize_t * dims, short * data);

        static void toScilabUInt16(void * pvApiCtx, const int position, const int ndims, const hsize_t * dims, unsigned short * data);

        static void toScilabInt32(void * pvApiCtx, const int position, const int ndims, const hsize_t * dims, int * data);

        static void toScilabUInt32(void * pvApiCtx, const int position, const int ndims, const hsize_t * dims, unsigned int * data);

#ifdef __SCILAB_INT64__

        static void toScilabInt64(void * pvApiCtx, const int position, const int ndims, const hsize_t * dims, long long * data);

        static void toScilabUInt64(void * pvApiCtx, const int position, const int ndims, const hsize_t * dims, unsigned long long * data);

#endif // __SCILAB_INT64__

        template <typename T>
        static void C2FHypermatrix(const int ndims, const hsize_t * dims, const hsize_t size, const T * src, T * dest)
            {
                if (ndims == 2)
                {
                    for (int i = 0; i < dims[0]; i++)
                    {
                        for (int j = 0; j < dims[1]; j++)
                        {
                            dest[i + dims[0] * j] = src[j + dims[1] * i];
                        }
                    }
                }
                else
                {
                    hsize_t * cumprod = new hsize_t[ndims];
                    hsize_t * cumdiv = new hsize_t[ndims];
                    cumprod[0] = 1;
                    cumdiv[ndims - 1] = 1;
                    for (int i = 0; i < ndims - 1; i++)
                    {
                        cumprod[i + 1] = dims[i] * cumprod[i];
                        cumdiv[i] = size / cumprod[i + 1];
                    }

                    reorder(ndims, dims, cumprod, cumdiv, src, dest);
                    delete[] cumprod;
                    delete[] cumdiv;
                }
            }

    private:

        static int * getHypermatrix(void * pvApiCtx, const int position, const int ndims, const hsize_t * dims);

        template <typename T>
        static void reorder(const int ndims, const hsize_t * dims, const hsize_t * cumprod, const hsize_t * cumdiv, const T * src, T * dest)
            {
                if (ndims == 1)
                {
                    for (int i = 0; i < *dims; i++)
                    {
                        *dest = src[i];
                        dest += *cumprod;
                    }
                }
                else
                {
                    for (int i = 0; i < *dims; i++)
                    {
                        reorder(ndims - 1, dims + 1, cumprod + 1, cumdiv + 1, src, dest);
                        dest += *cumprod;
                        src += *cumdiv;
                    }
                }
            }
    };
}

#endif // __H5DATACONVERTER_HXX__
