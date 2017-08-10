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

#ifndef __H5DATAFACTORY_HXX__
#define __H5DATAFACTORY_HXX__

#include "HDF5Objects.h"
#include "H5Object.hxx"
#include "H5Data.hxx"
#include "H5StringData.hxx"
#include "H5TimeData.hxx"
#include "H5BitfieldData.hxx"
#include "H5ReferenceData.hxx"
#include "H5CompoundData.hxx"
#include "H5ArrayData.hxx"
#include "H5OpaqueData.hxx"
#include "H5TransformedData.hxx"
#include "H5EnumData.hxx"
#include "H5VlenData.hxx"
#include "H5CharData.hxx"
#include "H5UnsignedCharData.hxx"

namespace org_modules_hdf5
{

class H5DataFactory
{

public:

    static H5Data & getData(H5Object & parent, const hid_t obj, H5Dataspace * space, hsize_t * selectdims, const bool isAttribute);

    static H5Data & getObjectData(H5Object & parent, const hsize_t totalSize, const hsize_t dataSize, const hid_t type, const hsize_t ndims, const hsize_t * dims, void * data, const hsize_t stride, const size_t offset, const bool dataOwner);

private:

    static void getNativeData(const hid_t obj, const hid_t space, hsize_t * selectdims, const hid_t type, hsize_t * totalSize, hsize_t * dataSize, hsize_t * ndims, hsize_t ** dims, void ** data, const bool isAttribute);
};
}

#endif // __H5DATAFACTORY_HXX__
