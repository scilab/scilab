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

#ifndef __H5DATASPACE_HXX__
#define __H5DATASPACE_HXX__

#include "HDF5Objects.h"
#include "H5Object.hxx"

namespace org_modules_hdf5
{

class H5Dataspace : public H5Object
{
    hid_t space;

public:

    H5Dataspace(H5Object & _parent, hid_t _space);
    ~H5Dataspace();

    virtual hid_t getH5Id() const;
    virtual std::vector<unsigned int> getDims(const bool b) const;
    virtual std::string getTypeName() const;

    virtual std::string dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel) const;
    virtual std::string toString(const unsigned int indentLevel) const;
    virtual std::string getStringDims() const;

    virtual void getAccessibleAttribute(const std::string & name, const int pos, void * pvApiCtx) const;

    void select() const;

    virtual bool isDataspace() const
    {
        return true;
    }

    template <typename T>
    hsize_t * select(const unsigned int size, const T * start, const T * stride, const T * count, const T * block) const
    {
        return select(space, size, start, stride, count, block);
    }

    template <typename T>
    static hsize_t * select(const hid_t space, const unsigned int size, const T * start, const T * stride, const T * count, const T * block)
    {
        if (!start)
        {
            H5Sselect_all(space);
            return 0;
        }

        if (H5Sget_simple_extent_ndims(space) != size)
        {
            throw H5Exception(__LINE__, __FILE__, _("Invalid selection rank."));
        }

        herr_t err;
        hsize_t * hstart = new hsize_t[size];
        hsize_t * hstride = new hsize_t[size];
        hsize_t * hcount = new hsize_t[size];
        hsize_t * hblock = new hsize_t[size];
        hsize_t * dims = new hsize_t[size];

        for (unsigned int i = 0; i < size; i++)
        {
            hstart[i] = (hsize_t)start[i] - 1;
            hstride[i] = stride ? (hsize_t)stride[i] : 1;
            hblock[i] = block ? (hsize_t)block[i] : 1;
            hcount[i] = (hsize_t)count[i];
            dims[i] = hblock[i] * hcount[i];
        }

        err = H5Sselect_hyperslab(space, H5S_SELECT_SET, hstart, hstride, hcount, hblock);
        delete[] hstart;
        delete[] hstride;
        delete[] hcount;
        delete[] hblock;
        if (err < 0)
        {
            delete[] dims;
            throw H5Exception(__LINE__, __FILE__, _("Invalid selection."));
        }

        if (H5Sselect_valid(space) <= 0)
        {
            H5Sselect_all(space);
            delete[] dims;
            throw H5Exception(__LINE__, __FILE__, _("Invalid selection."));
        }

        return dims;
    }

    template <typename T>
    void select(const unsigned int size, const T * coords) const
    {
        herr_t err;
        hsize_t * hcoords = new hsize_t[size];
        H5S_seloper_t selop = H5S_SELECT_SET;

        err = H5Sselect_elements(space, selop, (size_t)size, hcoords);
        delete[] hcoords;
        if (err)
        {
            throw H5Exception(__LINE__, __FILE__, _("Invalid selection."));
        }

        if (H5Sselect_valid(space) <= 0)
        {
            H5Sselect_all(space);
            throw H5Exception(__LINE__, __FILE__, _("Invalid selection."));
        }
    }

    static H5Dataspace & createDataspace(H5Object & parent, const std::string & type);
    static H5Dataspace & createDataspace(H5Object & parent, const int rank, const hsize_t * dims, const hsize_t * maxdims);
};
}

#endif // __H5DATASPACE_HXX__
