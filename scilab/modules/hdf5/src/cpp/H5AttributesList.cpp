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

#include "H5AttributesList.hxx"
#include "H5Attribute.hxx"

namespace org_modules_hdf5
{

H5AttributesList::H5AttributesList(H5Object & _parent) : H5ListObject<H5Attribute>(_parent) { }

H5AttributesList::H5AttributesList(H5Object & _parent, const unsigned int _size, const unsigned int * _index) : H5ListObject<H5Attribute>(_parent, _size, _index) { }

H5AttributesList::~H5AttributesList() { }

unsigned int H5AttributesList::getSize() const
{
    if (H5ListObject<H5Attribute>::indexList)
    {
        return H5ListObject<H5Attribute>::indexSize;
    }
    else
    {
        H5O_info_t info;
        herr_t err = H5Oget_info(parent.getH5Id(), &info);

        if (err < 0)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot get the size of attribute list."));
        }

        return (unsigned int)info.num_attrs;
    }
}

void H5AttributesList::setObject(const unsigned int pos, H5Attribute & attribute)
{

}

H5Attribute & H5AttributesList::getObject(const std::string & name)
{
    return *new H5Attribute(parent, name);
}

H5Attribute & H5AttributesList::getObject(const int pos)
{
    return getObject(pos, true);
}

H5Attribute & H5AttributesList::getObject(const int pos, const bool checkPos)
{
    hid_t attr;
    ssize_t ssize;
    std::string name;
    int _pos = pos;

    if (checkPos)
    {
        unsigned int size = getSize();
        if (pos < 0 || pos >= (int)size)
        {
            throw H5Exception(__LINE__, __FILE__, _("Invalid index %d: must be between 0 and %d."), pos, size);
        }
    }

    if (H5ListObject<H5Attribute>::indexList)
    {
        _pos = H5ListObject<H5Attribute>::indexList[pos];
    }

    attr = H5Aopen_by_idx(parent.getH5Id(), ".", H5_INDEX_NAME, H5_ITER_NATIVE, (hsize_t)_pos, H5P_DEFAULT, H5P_DEFAULT);
    if (attr < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot open attribute at position %d."), pos);
    }

    ssize = H5Aget_name(attr, 0, 0);
    if (ssize > 0)
    {
        char * _name = new char[ssize + 1];
        H5Aget_name(attr, ssize + 1, _name);
        name = std::string(_name);
        delete[] _name;
    }

    return *new H5Attribute(parent, attr, name);
}

std::string H5AttributesList::dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel) const
{
    std::ostringstream os;
    unsigned int size = getSize();

    for (unsigned int i = 0; i < size; i++)
    {
        const H5Attribute & attr = const_cast<H5AttributesList *>(this)->getObject(i, false);
        os << attr.dump(alreadyVisited, indentLevel);

        delete &attr;
    }

    return os.str();
}

std::string H5AttributesList::toString(const unsigned int indentLevel) const
{
    std::ostringstream os;
    std::string indentString = H5Object::getIndentString(indentLevel);

    os << indentString << _("Filename") << ": " << getFile().getFileName() << std::endl
       << indentString << _("Number of elements") << ": " << getSize();

    return os.str();
}
}
