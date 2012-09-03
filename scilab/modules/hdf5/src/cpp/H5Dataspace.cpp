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

#include "H5Dataspace.hxx"

namespace org_modules_hdf5
{
    
    H5Dataspace::H5Dataspace(H5Object & _parent, hid_t _space) : H5Object(_parent), space(_space)
    {
    }

    H5Dataspace::~H5Dataspace()
    {
	if (space >= 0)
	{
	    H5Sclose(space);
	}
    }

    hid_t H5Dataspace::getH5Id()
    {
	return space;
    }
    
    std::string H5Dataspace::dump(unsigned int indentLevel) const
    {
	std::ostringstream os;
	hsize_t dims[64];
	hsize_t maxdims[64];
	int ndims;
	H5S_class_t _class = H5Sget_simple_extent_type(space);
	
	os << H5Object::getIndentString(indentLevel);

	switch (_class)
	{
	case H5S_SCALAR:
	    os << "DATASPACE SCALAR";
	    break;
	case H5S_SIMPLE:
	    os << "DATASPACE SIMPLE { ";
	    ndims = H5Sget_simple_extent_dims(space, (hsize_t *)dims, (hsize_t *)maxdims);
	    os << "( ";
	    for (int i = 0; i < ndims - 1; i++)
	    {
		os << dims[i] << ", ";
	    }
	    os << dims[ndims -1] << " ) / ( ";
	    for (int i = 0; i < ndims - 1; i++)
	    {
		if (maxdims[i] == H5S_UNLIMITED)
		{
		    os << "H5S_UNLIMITED, ";
		}
		else
		{
		    os << maxdims[i] << ", ";
		}
	    }
	    if (maxdims[ndims - 1] == H5S_UNLIMITED)
	    {
		os << "H5S_UNLIMITED ) }";
	    }
	    else
	    {
		os << maxdims[ndims - 1] << " ) }";
	    }
	    break;
	case H5S_NULL:
	    os << "DATASPACE NULL";
	    break;
	case H5S_NO_CLASS:
	default:
	    os << _("Unknown dataspace");
	}
	
	os << std::endl;

	return os.str();
    }

    std::string H5Dataspace::toString(unsigned int indentLevel) const
    {
	return "";
    }
}
