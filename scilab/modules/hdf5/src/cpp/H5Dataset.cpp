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
#include "H5Attribute.hxx"
#include "H5HardLink.hxx"

namespace org_modules_hdf5
{

void H5Dataset::init()
{
    dataset = H5Dopen2(getParent().getH5Id(), name.c_str(), H5P_DEFAULT);
    if (dataset < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot open the given dataset %s."), name.c_str());
    }
}

H5Dataset::H5Dataset(H5Object & _parent, const std::string & _name) : H5Object(_parent, _name), dataset((hid_t) - 1)
{
    init();
}

H5Dataset::H5Dataset(H5Object & _parent, hid_t _dataset, const std::string & _name) : H5Object(_parent, _name), dataset(_dataset)
{

}

H5Dataset::~H5Dataset()
{
    if (dataset >= 0)
    {
        H5Dclose(dataset);
    }
}

H5Data & H5Dataset::getData()
{
    return H5DataFactory::getData(*this, dataset, 0, 0, false);
}

H5Data & H5Dataset::getData(H5Dataspace & space, hsize_t * dims)
{
    return H5DataFactory::getData(*this, dataset, &space, dims, false);
}

H5Dataspace & H5Dataset::getSpace()
{
    hid_t space = H5Dget_space(dataset);
    if (space < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot get the dataspace associated with dataset named %s."), name.c_str());
    }

    return *new H5Dataspace(*this, space);
}

H5Type & H5Dataset::getDataType()
{
    hid_t type = H5Dget_type(dataset);
    if (type < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot get the dataspace associated with dataset named %s."), name.c_str());
    }

    return *new H5Type(*this, type);
}

H5Dataset::H5Layout & H5Dataset::getLayout()
{
    hid_t plist = H5Dget_create_plist(dataset);
    H5D_layout_t layout = H5Pget_layout(plist);
    H5Dataset::H5Layout * obj = 0;

    switch (layout)
    {
        case H5D_COMPACT:
            obj = new H5Dataset::H5CompactLayout(*this);
            break;
        case H5D_CONTIGUOUS:
            obj = new H5Dataset::H5ContiguousLayout(*this);
            break;
        case H5D_CHUNKED:
            obj = new H5Dataset::H5ChunkedLayout(*this);
            break;
        default:
            H5Pclose(plist);
            throw H5Exception(__LINE__, __FILE__, _("Invalid layout"));
    }

    H5Pclose(plist);

    return *obj;
}

void H5Dataset::getAccessibleAttribute(const std::string & _name, const int pos, void * pvApiCtx) const
{
    SciErr err;
    std::string lower(_name);

    try
    {
        H5Attribute * attr = new H5Attribute(*const_cast<H5Dataset *>(this), _name);
        attr->createOnScilabStack(pos, pvApiCtx);

        return;
    }
    catch (const H5Exception & e) { }

    std::transform(_name.begin(), _name.end(), lower.begin(), tolower);

    if (lower == "attributes")
    {
        const H5AttributesList & attrs = const_cast<H5Dataset *>(this)->getAttributes();
        attrs.createOnScilabStack(pos, pvApiCtx);

        return;
    }
    else if (lower == "type")
    {
        const H5Type & type = const_cast<H5Dataset *>(this)->getDataType();
        type.createOnScilabStack(pos, pvApiCtx);

        return;
    }
    else if (lower == "dataspace")
    {
        const H5Dataspace & space = const_cast<H5Dataset *>(this)->getSpace();
        space.createOnScilabStack(pos, pvApiCtx);

        return;
    }
    else if (lower == "data")
    {
        const H5Data & data = const_cast<H5Dataset *>(this)->getData();
        data.toScilab(pvApiCtx, pos);

        if (!data.isReference())
        {
            delete &data;
        }

        return;
    }
    else if (lower == "layout")
    {
        const H5Dataset::H5Layout & layout = const_cast<H5Dataset *>(this)->getLayout();
        layout.createOnScilabStack(pos, pvApiCtx);

        return;
    }

    H5Object::getAccessibleAttribute(_name, pos, pvApiCtx);
}

std::string H5Dataset::dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel) const
{
    std::ostringstream os;
    haddr_t addr = this->getAddr();
    std::map<haddr_t, std::string>::iterator it = alreadyVisited.find(addr);
    if (it != alreadyVisited.end())
    {
        os << H5Object::getIndentString(indentLevel) << "DATASET \"" << getName() << "\" {" << std::endl
           << H5Object::getIndentString(indentLevel + 1) << "HARDLINK \"" << it->second << "\"" << std::endl
           << H5Object::getIndentString(indentLevel) << "}" << std::endl;

        return os.str();
    }
    else
    {
        alreadyVisited.insert(std::pair<haddr_t, std::string>(addr, getCompletePath()));
    }

    const H5Type & type = const_cast<H5Dataset *>(this)->getDataType();
    const H5Dataspace & space = const_cast<H5Dataset *>(this)->getSpace();
    const H5AttributesList & attrs = const_cast<H5Dataset *>(this)->getAttributes();
    const H5Data & data = const_cast<H5Dataset *>(this)->getData();
    const H5Dataset::H5Layout & layout = const_cast<H5Dataset *>(this)->getLayout();

    os << H5Object::getIndentString(indentLevel) << "DATASET \"" << getName() << "\" {" << std::endl
       << type.dump(alreadyVisited, indentLevel + 1)
       << space.dump(alreadyVisited, indentLevel + 1)
       << layout.dump(alreadyVisited, indentLevel + 1)
       << data.dump(alreadyVisited, indentLevel + 1)
       << attrs.dump(alreadyVisited, indentLevel + 1)
       << H5Object::getIndentString(indentLevel) << "}" << std::endl;

    delete &type;
    delete &space;
    delete &data;
    delete &attrs;
    delete &layout;

    return os.str();
}

void H5Dataset::printLsInfo(std::ostringstream & os) const
{
    const H5Dataspace & space = const_cast<H5Dataset *>(this)->getSpace();
    std::vector<unsigned int> dims = space.getDims();
    std::string str(getName());
    H5Object::getResizedString(str);

    os << str << "Dataset {";

    if (dims.size() == 0)
    {
        os << "}";
    }
    else
    {
        for (unsigned int i = 0; i < dims.size() - 1; i++)
        {
            os << dims[i] << ", ";
        }
        os << dims[dims.size() - 1] << "}";
    }

    delete &space;

    os << std::endl;
}

std::string H5Dataset::ls() const
{
    std::ostringstream os;
    printLsInfo(os);

    return os.str();
}

void H5Dataset::ls(std::vector<std::string> & name, std::vector<std::string> & type) const
{
    herr_t err;
    OpDataGetLs opdata;
    opdata.parent = const_cast<H5Dataset *>(this);
    opdata.name = &name;
    opdata.type = &type;
    hsize_t idx = 0;

    err = H5Aiterate2(dataset, H5_INDEX_NAME, H5_ITER_INC, &idx, H5Object::getLsAttributes, &opdata);
    if (err < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot list dataset attributes."));
    }
}

std::string H5Dataset::toString(const unsigned int indentLevel) const
{
    std::ostringstream os;
    std::string indentString = H5Object::getIndentString(indentLevel + 1);
    const H5Type & type = const_cast<H5Dataset *>(this)->getDataType();
    const H5AttributesList & attrs = const_cast<H5Dataset *>(this)->getAttributes();

    os << H5Object::getIndentString(indentLevel) << "HDF5 Dataset" << std::endl
       << indentString << _("Filename") << ": " << getParent().getFile().getFileName() << std::endl
       << indentString << _("Name") << ": " << getName() << std::endl
       << indentString << _("Path") << ": " << getCompletePath() << std::endl
       << indentString << _("Elements type") << ": " << type.getTypeName() << std::endl
       << indentString << _("Attributes") << ": [1 x " << attrs.getSize() << "]";

    delete &type;
    delete &attrs;

    return os.str();
}

hid_t H5Dataset::create(H5Object & loc, const std::string & name, const hid_t type, const hid_t targettype, const hid_t srcspace, const hid_t targetspace, void * data)
{
    herr_t err;
    hid_t dataset = H5Dcreate2(loc.getH5Id(), name.c_str(), targettype, targetspace == -1 ? srcspace : targetspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    if (dataset < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot create a new dataset."));
    }

    err = H5Dwrite(dataset, type, srcspace, H5S_ALL, H5P_DEFAULT, data);
    if (err < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot write data in the dataset."));
    }

    return dataset;
}
}
