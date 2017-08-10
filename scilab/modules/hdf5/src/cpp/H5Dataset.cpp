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
    std::string lower(_name);

    std::transform(_name.begin(), _name.end(), lower.begin(), tolower);

    if (lower == "attributes")
    {
        std::vector<std::string> names;
        getNames(*this, names, ATTRIBUTE);
        H5BasicData<char>::putStringVectorOnStack(names, (int)names.size(), 1, pos, pvApiCtx);

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
        data.toScilab(pvApiCtx, pos, 0, 0, H5Options::isReadFlip());

        if (data.mustDelete())
        {
            delete &data;
        }

        return;
    }
    /*else if (lower == "layout")
    {
        const H5Dataset::H5Layout & layout = const_cast<H5Dataset *>(this)->getLayout();
        layout.createOnScilabStack(pos, pvApiCtx);

        return;
    }*/
    else
    {
        try
        {
            H5Object & obj = H5Object::getObject(*const_cast<H5Dataset *>(this), _name);
            obj.createOnScilabStack(pos, pvApiCtx);
            return;
        }
        catch (const H5Exception & /*e*/) { }
    }

    H5Object::getAccessibleAttribute(_name, pos, pvApiCtx);
}

void H5Dataset::label(const unsigned int size, const unsigned int * dim, const char ** names) const
{
    hsize_t dims[__SCILAB_HDF5_MAX_DIMS__];
    unsigned int ndims;
    hid_t space = H5Dget_space(dataset);
    if (space < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot get the dataspace associated with dataset named %s."), name.c_str());
    }

    ndims = H5Sget_simple_extent_dims(space, (hsize_t *)dims, 0);
    H5Sclose(space);

    for (unsigned int i = 0; i < size; i++)
    {
        if (dim[i] > ndims)
        {

            throw H5Exception(__LINE__, __FILE__, _("Only %d dimensions."), ndims);
        }
        H5DSset_label(dataset, dim[i], names[i]);
    }
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
    const H5Dataset::H5Layout & layout = const_cast<H5Dataset *>(this)->getLayout();

    H5Data * data = 0;

    try
    {
        data = &const_cast<H5Dataset *>(this)->getData();
    }
    catch (const H5Exception & /*e*/)
    {

    }

    os << H5Object::getIndentString(indentLevel) << "DATASET \"" << getName() << "\" {" << std::endl
       << type.dump(alreadyVisited, indentLevel + 1)
       << space.dump(alreadyVisited, indentLevel + 1)
       << layout.dump(alreadyVisited, indentLevel + 1);

    if (data)
    {
        os << data->dump(alreadyVisited, indentLevel + 1);
    }
    else
    {
        os << H5Object::getIndentString(indentLevel + 1) << _("Error in retrieving data.") << std::endl;
    }

    os << attrs.dump(alreadyVisited, indentLevel + 1)
       << H5Object::getIndentString(indentLevel) << "}" << std::endl;

    delete &type;
    delete &space;
    if (data)
    {
        delete data;
    }
    delete &attrs;
    delete &layout;

    return os.str();
}

void H5Dataset::printLsInfo(std::ostringstream & os) const
{
    const H5Dataspace & space = const_cast<H5Dataset *>(this)->getSpace();
    std::vector<unsigned int> dims = space.getDims(true);
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
    OpDataGetLs opdata(const_cast<H5Dataset *>(this), &name, &type);
    hsize_t idx = 0;

    err = H5Aiterate(dataset, H5_INDEX_NAME, H5_ITER_INC, &idx, H5Object::getLsAttributes, &opdata);
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
    const H5Dataspace & space = const_cast<H5Dataset *>(this)->getSpace();

    os << H5Object::getIndentString(indentLevel) << "HDF5 Dataset" << std::endl
       << indentString << "Filename" << ": " << getParent().getFile().getFileName() << std::endl
       << indentString << "Name" << ": " << getName() << std::endl
       << indentString << "Path" << ": " << getCompletePath() << std::endl
       << indentString << "Type" << ": " << type.getTypeName() << std::endl
       << indentString << "Dataspace" << ": " << space.getTypeName() << std::endl
       << indentString << "Data" << ": " << space.getStringDims() << std::endl
       << indentString << "Attributes" << ": [1 x " << attrs.getSize() << "]";

    delete &space;
    delete &type;
    delete &attrs;

    return os.str();
}

bool H5Dataset::isChunked() const
{
    hid_t plist = H5Dget_create_plist(dataset);
    H5D_layout_t layout = H5Pget_layout(plist);
    bool chunked = layout == H5D_CHUNKED;
    H5Pclose(plist);

    return chunked;
}

hid_t H5Dataset::create(H5Object & loc, const std::string & name, const hid_t type, const hid_t targettype, const hid_t srcspace, const hid_t targetspace, void * data, const bool chunked)
{
    herr_t err;
    hid_t dataset;

    if (H5Lexists(loc.getH5Id(), name.c_str(), H5P_DEFAULT) > 0)
    {
        dataset = H5Oopen(loc.getH5Id(), name.c_str(), H5P_DEFAULT);
        if (dataset < 0)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot open the dataset: %s"), name.c_str());
        }

        if (targetspace > 0)
        {
            hid_t space = H5Dget_space(dataset);
            if (space < 0)
            {
                throw H5Exception(__LINE__, __FILE__, _("Cannot get the dataspace associated with dataset named %s."), name.c_str());
            }

            hsize_t * dims = 0;
            hsize_t * ddims = 0;
            hsize_t * maxdims = 0;
            hsize_t * dmaxdims = 0;

            try
            {
                herr_t err;
                int ndims = H5Sget_simple_extent_ndims(space);
                if (ndims < 0)
                {
                    throw H5Exception(__LINE__, __FILE__, _("Invalid source space"));
                }
                int dndims = H5Sget_simple_extent_ndims(targetspace);
                if (dndims < 0)
                {
                    throw H5Exception(__LINE__, __FILE__, _("Invalid target space"));
                }
                hsize_t * dims = new hsize_t[ndims];
                hsize_t * ddims = new hsize_t[dndims];
                hsize_t * maxdims = new hsize_t[ndims];
                hsize_t * dmaxdims = new hsize_t[dndims];

                H5Sget_simple_extent_dims(space, dims, maxdims);
                H5Sget_simple_extent_dims(targetspace, ddims, dmaxdims);
                H5Sclose(space);

                if (ndims != dndims)
                {
                    throw H5Exception(__LINE__, __FILE__, _("Wrong dimensions."));
                }
                else
                {
                    for (int i = 0; i < ndims; i++)
                    {
                        if (maxdims[i] != dmaxdims[i])
                        {
                            throw H5Exception(__LINE__, __FILE__, _("Cannot modify maximum dimensions."));
                        }
                        if (ddims[i] > dims[i])
                        {
                            err = H5Dset_extent(dataset, ddims);
                            if (err < 0)
                            {
                                throw H5Exception(__LINE__, __FILE__, _("Cannot modify dimension %d."), i);
                            }
                            break;
                        }
                    }
                }

                delete[] dims;
                delete[] ddims;
                delete[] maxdims;
                delete[] dmaxdims;

            }
            catch (const H5Exception & /*e*/)
            {
                delete[] dims;
                delete[] ddims;
                delete[] maxdims;
                delete[] dmaxdims;
                throw;
            }
        }
    }
    else
    {
        if (chunked)
        {
            herr_t err;
            int ndims = H5Sget_simple_extent_ndims(targetspace);
            if (ndims < 0)
            {
                throw H5Exception(__LINE__, __FILE__, _("Invalid target space"));
            }

            hsize_t * dims = new hsize_t[ndims];

            H5Sget_simple_extent_dims(targetspace, dims, 0);
            hid_t dcpl = H5Pcreate(H5P_DATASET_CREATE);
            H5Pset_layout(dcpl, H5D_CHUNKED);
            err = H5Pset_chunk(dcpl, ndims, dims);
            delete[] dims;

            if (err < 0)
            {
                H5Pclose(dcpl);
                throw H5Exception(__LINE__, __FILE__, _("Cannot set the chunk dimensions: %s"), name.c_str());
            }

            dataset = H5Dcreate(loc.getH5Id(), name.c_str(), targettype, targetspace == -1 ? srcspace : targetspace, H5P_DEFAULT, dcpl, H5P_DEFAULT);
            H5Pclose(dcpl);
        }
        else
        {
            dataset = H5Dcreate(loc.getH5Id(), name.c_str(), targettype, targetspace == -1 ? srcspace : targetspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        }

        if (dataset < 0)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot create the dataset: %s"), name.c_str());
        }
    }

    err = H5Dwrite(dataset, type, srcspace, targetspace == -1 ? H5S_ALL : targetspace, H5P_DEFAULT, data);
    if (err < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot write data in the dataset."));
    }

    return dataset;
}
}
