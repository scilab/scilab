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

#include "H5Dataset.hxx"
#include "H5DataFactory.hxx"

namespace org_modules_hdf5
{
    
    H5Dataset::H5Dataset(H5Object & _parent, const char * _name) : H5Object(_parent), name(_name), dataset((hid_t)-1)
    {
	dataset = H5Dopen2(_parent.getH5Id(), name, H5P_DEFAULT);
	if (dataset < 0)
	{
	    throw H5Exception(__LINE__, __FILE__, _("Cannot open the given dataset %s."), name);
	}
    }

    H5Dataset::H5Dataset(H5Object & _parent, hid_t _dataset, const char * _name) : H5Object(_parent), dataset(_dataset), name(_name)
    {

    }

    H5Dataset::~H5Dataset()
    {
	if (dataset >= 0)
	{
	    H5Dclose(dataset);
	}
	if (name)
	{
	    FREE(const_cast<char *>(name));
	}
    }

    H5Data & H5Dataset::getData()
    {
	return H5DataFactory::getData(*this, dataset, false);
    }
    
    H5Dataspace & H5Dataset::getSpace()
    {
	hid_t space = H5Dget_space(dataset);
	if (space < 0)
	{
	    throw H5Exception(__LINE__, __FILE__, _("Cannot get the dataspace associated with dataset named %s."), name);
	}
	
	return *new H5Dataspace(*this, space);
    }

    H5Type & H5Dataset::getDataType()
    {
	hid_t type = H5Dget_type(dataset);
	if (type < 0)
	{
	    throw H5Exception(__LINE__, __FILE__, _("Cannot get the dataspace associated with dataset named %s."), name);
	}
	
	return *new H5Type(*this, type);
    }

    std::string H5Dataset::dump(const unsigned int indentLevel) const
    { 
	std::ostringstream os;
	const H5Type & type = const_cast<H5Dataset *>(this)->getDataType();
	const H5Dataspace & space = const_cast<H5Dataset *>(this)->getSpace();
	const H5AttributesList & attrs = const_cast<H5Dataset *>(this)->getAttributes();
	const H5Data & data = const_cast<H5Dataset *>(this)->getData();

	os << H5Object::getIndentString(indentLevel) << "DATASET \"" << name << "\" {" << std::endl
	    << type.dump(indentLevel + 1)
	    << space.dump(indentLevel + 1)
	    << data.dump(indentLevel + 1)
	    << attrs.dump(indentLevel + 1)
	   << H5Object::getIndentString(indentLevel) << "}" << std::endl;

	delete &type;
	delete &space;
	delete &data;
	delete &attrs;
	
	return os.str();
    } 

    std::string H5Dataset::toString(const unsigned int indentLevel) const
    { 
	return "";
    }
}
