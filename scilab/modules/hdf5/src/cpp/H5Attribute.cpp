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

#include "H5Options.hxx"
#include "H5Attribute.hxx"
#include "H5DataFactory.hxx"

namespace org_modules_hdf5
{

H5Attribute::H5Attribute(H5Object & _parent, const std::string & _name) : H5Object(_parent, _name)
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

H5Attribute::H5Attribute(H5Object & _parent, hid_t _attr, const std::string & _name) : H5Object(_parent, _name), attr(_attr)
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
    return H5DataFactory::getData(*this, attr, 0, 0, true);
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
        data.toScilab(pvApiCtx, pos, 0, 0, H5Options::isReadFlip());

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
    H5Data * data = 0;

    try
    {
        data = &const_cast<H5Attribute *>(this)->getData();
    }
    catch (const H5Exception & /*e*/)
    {

    }

    os << H5Object::getIndentString(indentLevel) << "ATTRIBUTE \"" << getName() << "\" {" << std::endl
       << type.dump(alreadyVisited, indentLevel + 1)
       << space.dump(alreadyVisited, indentLevel + 1);

    if (data)
    {
        os << data->dump(alreadyVisited, indentLevel + 1);
    }
    else
    {
        os << H5Object::getIndentString(indentLevel + 1) << _("Error in retrieving data.") << std::endl;
    }

    os << H5Object::getIndentString(indentLevel) << "}" << std::endl;

    delete &type;
    delete &space;
    if (data)
    {
        delete data;
    }

    return os.str();
}

std::string H5Attribute::toString(const unsigned int indentLevel) const
{
    std::ostringstream os;
    const std::string indentString = H5Object::getIndentString(indentLevel + 1);
    const H5Type & type = const_cast<H5Attribute *>(this)->getDataType();
    const H5Dataspace & space = const_cast<H5Attribute *>(this)->getSpace();

    os << H5Object::getIndentString(indentLevel) << "HDF5 Attribute" << std::endl
       << indentString << "Filename" << ": " << getFile().getFileName() << std::endl
       << indentString << "Name" << ": " << getName() << std::endl
       << indentString << "Parent path" << ": " << getParent().getCompletePath() << std::endl
       << indentString << "Type" << ": " << type.getTypeName() << std::endl
       << indentString << "Dataspace" << ": " << space.getTypeName() << std::endl
       << indentString << "Data" << ": " << space.getStringDims() << std::endl;

    delete &type;
    delete &space;

    return os.str();
}

void H5Attribute::copy(H5Object & parent, const std::string & name)
{
    H5Attribute::copy(this->getH5Id(), parent.getH5Id(), name);
}

hid_t H5Attribute::create(H5Object & loc, const std::string & name, const hid_t type, const hid_t targettype, const hid_t srcspace, const hid_t targetspace, void * data, const bool chunked)
{
    return create(loc.getH5Id(), name, type, targettype, srcspace, targetspace, data);
}

hid_t H5Attribute::create(const hid_t loc, const std::string & name, const hid_t type, const hid_t targettype, const hid_t srcspace, const hid_t targetspace, void * data)
{
    herr_t err;
    if (H5Aexists(loc, name.c_str()) > 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Attribute %s already exists."), name.c_str());
    }

    hid_t attr = H5Acreate(loc, name.c_str(), targettype, targetspace == -1 ? srcspace : targetspace, H5P_DEFAULT, H5P_DEFAULT);
    if (attr < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot create a new attribute."));
    }

    err = H5Awrite(attr, type, data);
    if (err < 0)
    {
        H5Aclose(attr);
        throw H5Exception(__LINE__, __FILE__, _("Cannot write data in the attribute."));
    }

    return attr;
}

void H5Attribute::copy(const hid_t src, const hid_t dest, const std::string & name)
{
    hid_t type, stype;
    hid_t space, sspace;
    char * data = 0;
    hsize_t size;
    hsize_t * dims = 0;
    hsize_t ndims;

    sspace = H5Aget_space(src);
    if (sspace < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot copy the attribute"));
    }
    space = H5Scopy(sspace);
    H5Sclose(sspace);

    stype = H5Aget_type(src);
    if (stype < 0)
    {
        H5Sclose(space);
        throw H5Exception(__LINE__, __FILE__, _("Cannot copy the attribute"));
    }
    type = H5Tcopy(stype);
    H5Tclose(stype);

    size = H5Tget_size(type);
    dims = new hsize_t[__SCILAB_HDF5_MAX_DIMS__];
    ndims = H5Sget_simple_extent_dims(space, dims, 0);
    for (unsigned int i = 0; i < ndims; i++)
    {
        size *= dims[i];
    }

    data = new char[size];
    if (H5Aread(src, type, data) < 0)
    {
        H5Sclose(space);
        H5Tclose(type);
        delete[] dims;
        delete[] data;

        throw H5Exception(__LINE__, __FILE__, _("Cannot read attribute data."));
    }

    try
    {
        hid_t attr = create(dest, name, type, type, space, space, data);
        H5Aclose(attr);
        H5Sclose(space);
        H5Tclose(type);
        delete[] dims;
        delete[] data;
    }
    catch (const H5Exception & /*e*/)
    {
        H5Sclose(space);
        H5Tclose(type);
        delete[] dims;
        delete[] data;
        throw;
    }
}

}
