/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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

#ifndef __H5DATACONVERTER_HXX__
#define __H5DATACONVERTER_HXX__

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

    static std::string dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel, const int ndims, const hsize_t * dims, const H5Data & obj, const bool line = true)
    {
        std::ostringstream os;
        std::string indent = H5Object::getIndentString(indentLevel);
        unsigned int pos = 0;

        os.setf(std::ios::fixed, std::ios::floatfield);
        os.precision(1);

        os << indent << "DATA {" << std::endl;
        printData(indentLevel, indent + "(", os, ndims, dims, &pos, obj, line);
        os << indent << "}" << std::endl;

        return os.str();
    }

    static void printData(const unsigned int indentLevel, const std::string & start, std::ostringstream & os, const int ndims, const hsize_t * dims, unsigned int * pos, const H5Data & obj, const bool line)
    {
        std::string indent = H5Object::getIndentString(indentLevel);

        if (ndims == 0)
        {
            os << start << "0): ";
            obj.printData(os, 0, 0);
            os << std::endl;
        }
        else if (ndims == 1)
        {
            if (line)
            {
                os << start << "0): ";
                for (hsize_t i = 0; i < dims[0] - 1; i++)
                {
                    obj.printData(os, *pos + (int)i, 0);
                    os << ", ";
                }
                obj.printData(os, *pos + (int)dims[0] - 1, 0);
            }
            else
            {
                for (hsize_t i = 0; i < dims[0] - 1; i++)
                {
                    os << start << i << "): ";
                    obj.printData(os, *pos + (int)i, indentLevel);
                    os << ", " << std::endl;
                }
                os << start << dims[0] - 1 << "): ";
                obj.printData(os, *pos + (int)dims[0] - 1, indentLevel);
            }

            os << std::endl;
            *pos += (int)dims[0];
        }
        else
        {
            std::ostringstream oss;
            for (hsize_t i = 0; i < dims[0]; i++)
            {
                oss << start << (unsigned int)i << ",";
                printData(indentLevel, oss.str(), os, ndims - 1, dims + 1, pos, obj, line);
                oss.str("");
            }
        }
    }

    template <typename T>
    static void C2FHypermatrix(const int ndims, const hsize_t * dims, const hsize_t size, const T * src, T * dest, const bool flip = true)
    {
        if (flip)
        {
            hsize_t totalSize = 1;
            for (int i = 0; i < ndims; i++)
            {
                totalSize *= dims[i];
            }
            memcpy(dest, src, totalSize * sizeof(T));
        }
        else
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
