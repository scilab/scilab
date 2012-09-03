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

#include "H5AttributesList.hxx"
#include "H5Attribute.hxx"

namespace org_modules_hdf5
{
    
    H5AttributesList::H5AttributesList(H5Object & _parent) : H5ListObject(_parent) { }

    H5AttributesList::~H5AttributesList() { }

    const unsigned int H5AttributesList::getSize() const
    {
	H5O_info_t info;
	herr_t err = H5Oget_info(parent.getH5Id(), &info);
	
	if (err < 0)
	{
	    throw H5Exception(__LINE__, __FILE__, _("Cannot get the size of attribute list."));
	}
	
	return (unsigned int)info.num_attrs;
    }

    void H5AttributesList::setObject(const unsigned int pos, H5Attribute & attribute)
    {
	
    }

    H5Attribute & H5AttributesList::getObject(const int pos)
    {
	return getObject(pos, true);
    }

    H5Attribute & H5AttributesList::getObject(const int pos, const bool checkPos)
    {
	if (checkPos)
	{
	    unsigned int size = getSize();
	    if (pos < 0 || pos >= size)
	    {
		throw H5Exception(__LINE__, __FILE__, _("Invalid index %u: must be between 0 and %u."), pos, size);
	    }
	}

	return *new H5Attribute(parent, parent.getH5Id(), (const unsigned int)pos);
    }

    std::string H5AttributesList::dump(const unsigned int indentLevel) const
    {
	std::ostringstream os;
	unsigned int size = getSize();
	
	for (unsigned int i = 0; i < size; i++)
	{
	    const H5Attribute & attr = const_cast<H5AttributesList *>(this)->getObject(i, false);
	    os << attr.dump(indentLevel);

	    delete &attr;
	}
	
	return os.str();
    }

    std::string H5AttributesList::toString(const unsigned int indentLevel) const
    {
	std::ostringstream os;
	std::string indentString = H5Object::getIndentString(indentLevel);
	
	os << indentString << _("Filename") << ": " << getFile().getFileName() << std::endl
	   << indentString << _("Number of attributes") << ": " << getSize();

	return os.str();
    }
}
