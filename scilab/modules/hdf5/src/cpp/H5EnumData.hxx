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

#ifndef __H5ENUMDATA_HXX__
#define __H5ENUMDATA_HXX__

#include <map>
#include <string>
#include <vector>

#include "H5Data.hxx"

namespace org_modules_hdf5
{

template<typename T>
class H5EnumData : public H5BasicData<T>
{
    typedef std::map<T, std::string> MapType;
    const unsigned int nmembers;
    const std::string * names;
    MapType map;

public:

    H5EnumData(H5Object & _parent, const hsize_t _totalSize, const hsize_t _dataSize, const hsize_t _ndims, const hsize_t * _dims, T * _data, hid_t enumType, hid_t nativeType, const unsigned int _nmembers, const std::string * _names, const hsize_t _stride, const size_t _offset, const bool _dataOwner) : H5BasicData<T>(_parent, _totalSize, _dataSize, _ndims, _dims, _data, _stride, _offset, _dataOwner), nmembers(_nmembers), names(_names)
    {
        hid_t super = H5Tget_super(enumType);
        for (unsigned long long i = 0; i < nmembers; ++i)
        {
            T value = 0;
            H5Tget_member_value(enumType, i, &value);
            H5Tconvert(super, nativeType, 1, &value, 0, H5P_DEFAULT);
            map.insert(std::pair<T, std::string>(value, names[i]));
        }
    }

    virtual ~H5EnumData()
    {
        if (names)
        {
            delete[] names;
        }
    }

    virtual void printData(std::ostream & os, const unsigned int pos, const unsigned int indentLevel) const
    {
        typename MapType::const_iterator i = map.find(static_cast<T *>(H5BasicData<T>::getData())[pos]);
        os << i->second;
    }
};
}

#endif // __H5ENUMDATA_HXX__
