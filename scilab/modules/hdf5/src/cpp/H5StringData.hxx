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

#ifndef __H5STRINGDATA_HXX__
#define __H5STRINGDATA_HXX__

#include "H5BasicData.hxx"

namespace org_modules_hdf5
{

class H5StringData : public H5Data
{

protected:

    char ** transformedData;

public:

    H5StringData(H5Object & _parent, const hsize_t _totalSize, const hsize_t _stringSize, const hsize_t _ndims, const hsize_t * _dims, char * _data, const hsize_t _stride, const size_t _offset, const bool _dataOwner);

    H5StringData(H5Object & _parent, const hsize_t _totalSize, const hsize_t _stringSize, const hsize_t _ndims, const hsize_t * _dims, char ** _data, const hsize_t _stride, const size_t _offset, const bool _dataOwner);

    virtual ~H5StringData();

    virtual void * getData() const;

    virtual void printData(std::ostream & os, const unsigned int pos, const unsigned int indentLevel) const;

    virtual void toScilab(void * pvApiCtx, const int lhsPosition, int * parentList = 0, const int listPosition = 0, const bool flip = true) const;

    virtual std::string dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel) const;
};
}

#endif // __H5STRINGDATA_HXX__
