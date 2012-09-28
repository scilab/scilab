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
#include "H5OpaqueData.hxx"
#include "H5TransformedData.hxx"
#include "H5EnumData.hxx"
//#include "H5VlenData.hxx"
#include "H5CharData.hxx"
#include "H5UnsignedCharData.hxx"

namespace org_modules_hdf5
{

class H5DataFactory
{

public:

    static H5Data & getData(H5Object & parent, const hid_t obj, const bool isAttribute);

    static H5Data & getData(H5Object & parent, const hid_t obj, H5Dataspace * space, hsize_t * selectdims, const bool isAttribute);

private:

    static H5Data & getData(H5Object & parent, const hsize_t totalSize, const hid_t type, hsize_t ndims, hsize_t * dims, void * data, const hsize_t stride, const size_t offset, const bool dataOwner = false);

    static H5Data & getIntegerData(H5Object & parent, const hid_t obj, const hid_t space, hsize_t * selectdims, const hid_t type, const bool isAttribute);

    static H5Data & getFloatingData(H5Object & parent, const hid_t obj, const hid_t space, hsize_t * selectdims, const hid_t type, const bool isAttribute);

    static H5StringData & getStringData(H5Object & parent, const hid_t obj, const hid_t space, hsize_t * selectdims, const hid_t type, const bool isAttribute);

    static H5TimeData & getTimeData(H5Object & parent, const hid_t obj, const hid_t space, hsize_t * selectdims, const hid_t type, const bool isAttribute);

    static H5Data & getBitfieldData(H5Object & parent, const hid_t obj, const hid_t space, hsize_t * selectdims, const hid_t type, const bool isAttribute);

    static H5OpaqueData & getOpaqueData(H5Object & parent, const hid_t obj, const hid_t space, hsize_t * selectdims, const hid_t type, const bool isAttribute);

    static H5Data & getCompoundData(H5Object & parent, const hid_t obj, const hid_t space, hsize_t * selectdims, const hid_t type, const bool isAttribute);

    static H5ReferenceData & getReferenceData(H5Object & parent, const hid_t obj, const hid_t space, hsize_t * selectdims, const hid_t type, const bool isAttribute);

    static H5EnumData & getEnumData(H5Object & parent, const hid_t obj, const hid_t space, hsize_t * selectdims, const hid_t type, const bool isAttribute);

    //static H5VlenData & getVlenData(H5Object & parent, const hid_t obj, const hid_t space, hsize_t * selectdims, const hid_t type, const bool isAttribute);

    static void getNativeData(const hid_t obj, const hid_t space, hsize_t * selectdims, const hid_t type, hsize_t * totalSize, hsize_t * dataSize, hsize_t * ndims, hsize_t ** dims, void ** data, const bool isAttribute);
};
}

#endif // __H5DATAFACTORY_HXX__
