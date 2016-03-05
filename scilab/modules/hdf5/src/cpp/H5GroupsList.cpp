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

#include "H5GroupsList.hxx"

namespace org_modules_hdf5
{

H5GroupsList::H5GroupsList(H5Object & _parent) : H5ListObject<H5Group>(_parent) { }

H5GroupsList::~H5GroupsList() { }

const unsigned int H5GroupsList::getSize() const
{
    H5G_info_t info;
    hsize_t count = 0;
    hid_t parentId = getParent().getH5Id();
    herr_t err = H5Gget_info(parentId, &info);

    if (err < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot get the number of groups."));
    }

    for (hsize_t i = 0; i < info.nlinks; i++)
    {
        int type = H5Gget_objtype_by_idx(parentId, i);
        if (type == H5G_GROUP)
        {
            count++;
        }
    }

    return (unsigned int)count;
}

void H5GroupsList::setObject(const unsigned int pos, H5Group & obj)
{

}

H5Group & H5GroupsList::getObject(const int pos)
{
    return getObject(pos, true);
}

H5Group & H5GroupsList::getObject(const int pos, const bool checkPos)
{
    int type;
    herr_t err;
    hsize_t count = 0;
    hsize_t index = 0;
    hid_t parentId = getParent().getH5Id();
    ssize_t nameSize;
    char * name = 0;

    if (checkPos)
    {
        unsigned int size = getSize();
        if (pos < 0 || pos >= size)
        {
            throw H5Exception(__LINE__, __FILE__, _("Invalid index %d: must be between 0 and %d."), pos, size);
        }
    }

    err = H5Gget_info(parentId, &info);
    if (err < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot get the number of groups."));
    }

    for (; index < info.nlinks; index++)
    {
        int type = H5Gget_objtype_by_idx(parentId, index);
        if (type == H5G_GROUP)
        {
            count++;
            if (count == pos)
            {
                break;
            }
        }
    }

    nameSize = H5Gget_objname_by_idx(parentId, index, 0, 0);
    name = (char *)MALLOC((nameSize + 1) * sizeof(char));
    H5Gget_objname_by_idx(parentId, index, name, nameSize + 1);

    return *new H5Group(getParent(), name);
}

std::string H5GroupsList::dump(const unsigned int indentLevel) const
{
    std::ostringstream os;
    const unsigned int size = getSize();

    for (unsigned int i = 0; i < size; i++)
    {
        const H5Group & group = const_cast<H5GroupsList *>(this)->getObject(i, false);
        os << group.toString(indentLevel) << std::endl;

        delete &group;
    }

    return os.str();
}

std::string H5GroupsList::toString(const unsigned int indentLevel) const
{

}
}
