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

#include "H5LinksList.hxx"

namespace org_modules_hdf5
{
H5LinksList::H5LinksList(H5Object & _parent) : H5ListObject<H5Object>(_parent) { }

H5LinksList::~H5LinksList() { }

const unsigned int H5LinksList::getSize() const
{
    H5G_info_t info;
    herr_t err = H5Gget_info(getParent().getH5Id(), &info);

    if (err < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot get the number of links."));
    }

    return (unsigned int)info.nlinks;
}

void H5LinksList::setObject(const unsigned int pos, H5Object & obj)
{

}

H5Object & H5LinksList::getObject(const int pos)
{
    return getObject(pos, true);
}

H5Object & H5LinksList::getObject(const int pos, const bool checkPos)
{
    if (checkPos)
    {
        unsigned int size = getSize();
        if (pos < 0 || pos >= size)
        {
            throw H5Exception(__LINE__, __FILE__, _("Invalid index %d: must be between 0 and %d."), pos, size);
        }
    }

    H5Object * obj = 0;
    hid_t parentId = getParent().getH5Id();
    ssize_t nameSize = H5Gget_objname_by_idx(parentId, (hsize_t)pos, 0, 0);
    char * name = (char *)MALLOC((nameSize + 1) * sizeof(char));
    H5Gget_objname_by_idx(parentId, (hsize_t)pos, name, nameSize + 1);
    int type = H5Gget_objtype_by_idx(parentId, (hsize_t)pos);

    switch (type)
    {
        case H5G_LINK:
            obj = &H5Link::getLink(getParent(), name);
            break;
        case H5G_GROUP:
            obj = new H5Group(getParent(), name);
            break;
        case H5G_DATASET:
            obj = new H5Dataset(getParent(), name);
            break;
        case H5G_TYPE:
            obj = new H5Type(getParent(), name);
            break;
    }

    return *obj;
}

std::string H5LinksList::dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel) const
{
    std::ostringstream os;
    const unsigned int size = getSize();

    for (unsigned int i = 0; i < size; i++)
    {
        const H5Object & obj = const_cast<H5LinksList *>(this)->getObject(i, false);
        os << obj.dump(alreadyVisited, indentLevel);

        delete &obj;
    }

    return os.str();
}

std::string H5LinksList::toString(const unsigned int indentLevel) const
{
    return "";
}
}
