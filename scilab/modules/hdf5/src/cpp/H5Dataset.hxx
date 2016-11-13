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

#ifndef __H5DATASET_HXX__
#define __H5DATASET_HXX__

#include "H5Object.hxx"
#include "H5Data.hxx"
#include "H5Dataspace.hxx"
#include "H5Type.hxx"
#include "H5AttributesList.hxx"

#define __HDF5_SCILAB_NAME_LENGTH__ 256

namespace org_modules_hdf5
{
class H5Dataset : public H5Object
{
    hid_t dataset;

public :

    H5Dataset(H5Object & _parent, const std::string & _name);
    H5Dataset(H5Object & _parent, hid_t _dataset, const std::string & _name);

    virtual ~H5Dataset();

    hid_t getH5Id() const
    {
        return dataset;
    }

    virtual bool isDataset() const
    {
        return true;
    }

    H5Data & getData();
    H5Data & getData(H5Dataspace & space, hsize_t * dims);
    H5Dataspace & getSpace();
    H5Type & getDataType();
    bool isChunked() const;

    void label(const unsigned int size, const unsigned int * dim, const char ** name) const;

    virtual void getAccessibleAttribute(const std::string & name, const int pos, void * pvApiCtx) const;
    virtual std::string dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel) const;
    virtual std::string toString(const unsigned int indentLevel) const;
    virtual std::string ls() const;
    virtual void ls(std::vector<std::string> & name, std::vector<std::string> & type) const;
    virtual void printLsInfo(std::ostringstream & os) const;

    class H5Layout : public H5Object
    {

    public :

        H5Layout(H5Object & _parent) : H5Object(_parent) { }

        virtual hsize_t getStorageSize() const
        {
            return H5Dget_storage_size(parent.getH5Id());
        }

        virtual haddr_t getIOffset() const
        {
            return H5Dget_offset(parent.getH5Id());
        }

        virtual int getExternalCount() const
        {
            hid_t plist = H5Dget_create_plist(parent.getH5Id());
            int count = H5Pget_external_count(plist);

            H5Pclose(plist);
            return count;
        }

        virtual ~H5Layout() { }
    };

    class H5CompactLayout : public H5Layout
    {

    public :

        H5CompactLayout(H5Object & _parent) : H5Layout(_parent) { }

        virtual ~H5CompactLayout() { }

        virtual std::string dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel = 0) const
        {
            std::ostringstream os;
            std::string indent = H5Object::getIndentString(indentLevel);
            std::string indent1 = H5Object::getIndentString(indentLevel + 1);

            os << indent << "STORAGE_LAYOUT {" << std::endl
               << indent1 << "COMPACT" << std::endl
               << indent1 << "SIZE " << getStorageSize() << std::endl
               << indent << "}" << std::endl;

            return os.str();
        }
    };

    class H5ChunkedLayout : public H5Layout
    {

    public :

        H5ChunkedLayout(H5Object & _parent) : H5Layout(_parent) { }

        virtual ~H5ChunkedLayout() { }

        virtual std::string dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel = 0) const
        {
            std::ostringstream os;
            std::string indent = H5Object::getIndentString(indentLevel);
            std::string indent1 = H5Object::getIndentString(indentLevel + 1);
            // TODO: chunked est a finir (cf h5dump.c::2758)
            os << indent << "STORAGE_LAYOUT {" << std::endl
               << indent1 << "CHUNKED" << std::endl
               << indent1 << "SIZE " << getStorageSize() << std::endl
               << indent << "}" << std::endl;

            return os.str();
        }
    };

    class H5ContiguousLayout : public H5Layout
    {

    public :

        H5ContiguousLayout(H5Object & _parent) : H5Layout(_parent) { }

        virtual ~H5ContiguousLayout() { }

        virtual std::string dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel = 0) const
        {
            std::ostringstream os;
            std::string indent = H5Object::getIndentString(indentLevel);
            std::string indent1 = H5Object::getIndentString(indentLevel + 1);
            unsigned int extCount = (unsigned int)getExternalCount();

            os << indent << "STORAGE_LAYOUT {" << std::endl
               << indent1 << "CONTIGUOUS" << std::endl;

            if (extCount)
            {
                hid_t plist = H5Dget_create_plist(parent.getH5Id());
                for (unsigned int i = 0; i < extCount; i++)
                {
                    off_t offset;
                    hsize_t size;
                    char name[__HDF5_SCILAB_NAME_LENGTH__];
                    H5Pget_external(plist, i, __HDF5_SCILAB_NAME_LENGTH__, name, &offset, &size);
                    os << indent1 << "FILENAME " << name << " SIZE " << size << " OFFSET " << offset << std::endl;
                }
                H5Pclose(plist);
            }
            else
            {
                os << indent1 << "SIZE " << getStorageSize() << std::endl
                   << indent1 << "OFFSET " << getIOffset() << std::endl;
            }

            os << indent << "}" << std::endl;

            return os.str();
        }
    };

    H5Layout & getLayout();

    static hid_t create(H5Object & loc, const std::string & name, const hid_t type, const hid_t targettype, const hid_t srcspace, const hid_t targetspace, void * data, const bool chunked = false);

private:

    void init();
};
}

#endif // __H5DATASET_HXX__
