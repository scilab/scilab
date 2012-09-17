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

    virtual hid_t getH5Id();
    virtual std::vector<unsigned int> getDims() const;
    virtual std::string getTypeName() const;

    virtual std::string dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel) const;
    virtual std::string toString(const unsigned int indentLevel) const;

    virtual void getAccessibleAttribute(const std::string & name, const int pos, void * pvApiCtx) const;

    static H5Dataspace & createDataspace(H5Object & parent, const std::string & type);
    static H5Dataspace & createDataspace(H5Object & parent, const int rank, const hsize_t * dims, const hsize_t * maxdims);
};
}

#endif // __H5DATASPACE_HXX__
