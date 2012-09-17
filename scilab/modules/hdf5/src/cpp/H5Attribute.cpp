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

H5Attribute::H5Attribute(H5Object & _parent, const unsigned int pos) : H5Object(_parent), name("")
{
    attr = H5Aopen_by_idx(getParent().getH5Id(), ".", H5_INDEX_NAME, H5_ITER_NATIVE, (hsize_t)pos, H5P_DEFAULT, H5P_DEFAULT);
    if (attr < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot open attribute at position %u."), pos);
    }

    ssize_t size = H5Aget_name(attr, 0, 0);
    if (size > 0)
    {
        char * _name = new char[size + 1];
        H5Aget_name(attr, size + 1, _name);
        name = std::string(_name);
        delete[] _name;
    }
}

H5Attribute::H5Attribute(H5Object & _parent, const std::string & _name) : H5Object(_parent), name(_name)
{
    if (H5Aexists(getParent().getH5Id(), name.c_str()) <= 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot open attribute: %s"), name.c_str());
    }

    attr = H5Aopen(getParent().getH5Id(), name.c_str(), H5P_DEFAULT);
    if (attr < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot open attribute: %s"), name.c_str());
    }
}

H5Attribute::H5Attribute(H5Object & _parent, hid_t _attr, const std::string & _name) : H5Object(_parent), attr(_attr), name(_name)
{

}

H5Attribute::H5Attribute(H5Object & _parent, hid_t _attr, const char * _name) : H5Object(_parent), attr(_attr), name(std::string(_name))
{

}

H5Attribute::~H5Attribute()
{
    if (attr >= 0)
    {
        H5Aclose(attr);
    }
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

void H5Attribute::getAccessibleAttribute(const std::string & _name, const int pos, void * pvApiCtx) const
{
    SciErr err;
    std::string lower(_name);
    std::transform(_name.begin(), _name.end(), lower.begin(), tolower);

    if (lower == "type")
    {
        const H5Type & type = const_cast<H5Attribute *>(this)->getDataType();
        type.createOnScilabStack(pos, pvApiCtx);

        return;
    }
    else if (lower == "dataspace")
    {
        const H5Dataspace & space = const_cast<H5Attribute *>(this)->getSpace();
        space.createOnScilabStack(pos, pvApiCtx);

        return;
    }
    else if (lower == "data")
    {
        const H5Data & data = const_cast<H5Attribute *>(this)->getData();
        data.toScilab(pvApiCtx, pos);

        delete &data;

        return;
    }

    H5Object::getAccessibleAttribute(_name, pos, pvApiCtx);
}

std::string H5Attribute::dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel) const
{
    std::ostringstream os;
    const H5Type & type = const_cast<H5Attribute *>(this)->getDataType();
    const H5Dataspace & space = const_cast<H5Attribute *>(this)->getSpace();
    const H5Data & data = const_cast<H5Attribute *>(this)->getData();

    os << H5Object::getIndentString(indentLevel) << "ATTRIBUTE \"" << getName() << "\" {" << std::endl
       << type.dump(alreadyVisited, indentLevel + 1)
       << space.dump(alreadyVisited, indentLevel + 1)
       << data.dump(alreadyVisited, indentLevel + 1)
       << H5Object::getIndentString(indentLevel) << "}" << std::endl;

    delete &type;
    delete &space;
    delete &data;

    return os.str();
}

std::string H5Attribute::toString(const unsigned int indentLevel) const
{
    std::ostringstream os;
    const std::string indentString = H5Object::getIndentString(indentLevel + 1);
    const H5Type & type = const_cast<H5Attribute *>(this)->getDataType();

    os << H5Object::getIndentString(indentLevel) << "HDF5 Attribute" << std::endl
       << indentString << _("Filename") << ": " << getFile().getFileName() << std::endl
       << indentString << _("Attribute name") << ": " << getName() << std::endl
       << indentString << _("Attribute path") << ": " << getCompletePath() << std::endl
       << indentString << _("Value class") << ": " << type.getClassName();

    delete &type;

    return os.str();
}

hid_t H5Attribute::create(H5Object & loc, const std::string & name, hid_t type, hid_t targettype, hid_t space, void * data)
{
    herr_t err;
    hid_t attr = H5Acreate2(loc.getH5Id(), name.c_str(), targettype, space, H5P_DEFAULT, H5P_DEFAULT);
    if (attr < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot create a new attribute."));
    }

    err = H5Awrite(attr, type, data);
    if (err < 0)
    {
        H5Dclose(attr);
        throw H5Exception(__LINE__, __FILE__, _("Cannot write data in the attribute."));
    }

    return attr;
}
}
