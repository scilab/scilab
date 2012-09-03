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

#include "H5Attribute.hxx"
#include "H5DataFactory.hxx"

namespace org_modules_hdf5
{

    H5Attribute::H5Attribute(H5Object & _parent, const hid_t parentId, const unsigned int pos) : H5Object(_parent)
    {
        attr = H5Aopen_by_idx(parentId, ".", H5_INDEX_NAME, H5_ITER_NATIVE, (hsize_t)pos, H5P_DEFAULT, H5P_DEFAULT);
        if (attr < 0)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot open attribute at position %u."), pos);
        }

        ssize_t size = H5Aget_name(attr, 0, 0);
        if (size <= 0)
        {
            name = strdup("");
        }
        else
        {
	    char * _name = (char *)MALLOC(sizeof(char) * (size + 1));
            H5Aget_name(attr, size + 1, _name);
	    name = const_cast<const char *>(_name);
        }
    }

    H5Attribute::~H5Attribute()
    {
        if (attr >= 0)
        {
            H5Aclose(attr);
        }
        FREE(const_cast<char *>(name));
    }

    H5Data & H5Attribute::getData()
    {
	return H5DataFactory::getData(*this, attr, true);
    }

    H5Type & H5Attribute::getDataType()
    {
        hid_t type = H5Aget_type(attr);
        if (type < 0)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot get the attribute type"));
        }

        return *new H5Type(*this, type);
    }

    H5Dataspace & H5Attribute::getSpace()
    {
        hid_t space = H5Aget_space(attr);
        if (space < 0)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot get the attribute dataspace"));
        }

        return *new H5Dataspace(*this, space);
    }

    std::string H5Attribute::dump(const unsigned int indentLevel) const
    {
	std::ostringstream os;
	const H5Type & type = const_cast<H5Attribute *>(this)->getDataType();
	const H5Dataspace & space = const_cast<H5Attribute *>(this)->getSpace();
	const H5Data & data = const_cast<H5Attribute *>(this)->getData();

	os << H5Object::getIndentString(indentLevel) << "ATTRIBUTE \"" << name << "\" {" << std::endl
	   << type.dump(indentLevel + 1)
	   << space.dump(indentLevel + 1)
	   << data.dump(indentLevel + 1)
	   << H5Object::getIndentString(indentLevel) << "}" << std::endl;

	delete &type;
	delete &space;
	delete &data;

	return os.str();
    }

    std::string H5Attribute::toString(const unsigned int indentLevel) const
    {
	std::ostringstream os;
	const std::string indentString = H5Object::getIndentString(indentLevel);
	const H5Type & type = const_cast<H5Attribute *>(this)->getDataType();
	
	os << indentString << _("Filename") << ": " << getFile().getFileName() << std::endl
	   << indentString << _("Attribute name") << ": " << name << std::endl
	   << indentString << _("Attribute path") << ": " << getCompletePath() << std::endl
	   << indentString << _("Value class") << ": " << type.getClassName();

	delete &type;

	return os.str();
    }
}
