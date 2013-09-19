/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __H5REFERENCEDATA_HXX__
#define __H5REFERENCEDATA_HXX__

#include "H5File.hxx"
#include "H5Group.hxx"
#include "H5Dataset.hxx"
#include "H5Type.hxx"
#include "H5BasicData.hxx"

namespace org_modules_hdf5
{

class H5ReferenceData : public H5BasicData<char>
{

    const bool datasetReference;
    const hsize_t * cumprod;

public:

    H5ReferenceData(H5Object & _parent, const bool _datasetReference, const hsize_t _totalSize, const hsize_t _dataSize, const hsize_t _ndims, const hsize_t * _dims, char * _data, const hsize_t _stride, const size_t _offset, const bool _dataOwner);

    virtual ~H5ReferenceData();

    virtual void toScilab(void * pvApiCtx, const int lhsPosition, int * parentList = 0, const int listPosition = 0, const bool flip = true) const;

    bool isReference() const
    {
        return true;
    }
    const char ** getReferencesName() const;

    H5Object ** getReferencesObject() const;

    virtual H5Object & getData(const unsigned int size, const unsigned int * index) const;

    virtual std::string toString(const unsigned int indentLevel) const;

    virtual std::string dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel) const;

    virtual void printData(std::ostream & os, const unsigned int pos, const unsigned int indentLevel) const;

    virtual bool mustDelete() const
    {
        return false;
    }

    static void deleteReferencesObjects(H5Object ** objs, const unsigned int size);
};
}

#endif // __H5REFERENCEDATA_HXX__
