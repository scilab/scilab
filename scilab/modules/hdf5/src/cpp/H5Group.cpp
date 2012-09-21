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

#include "H5Group.hxx"
#include "H5File.hxx"
#include "H5SoftLinksList.hxx"
#include "H5LinksList.hxx"
#include "H5GroupsList.hxx"
#include "H5DatasetsList.hxx"
#include "H5TypesList.hxx"
#include "H5Link.hxx"

namespace org_modules_hdf5
{

void H5Group::init()
{
    group = H5Gopen(getParent().getH5Id(), name.c_str(), H5P_DEFAULT);
    if (group < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot open the group %s."), name.c_str());
    }
}

H5Group::H5Group(H5Object & _parent, const std::string & _name) : H5Object(_parent), name(_name)
{
    init();
}

H5Group::H5Group(H5Object & _parent, const char * _name) : H5Object(_parent), name(std::string(_name))
{
    init();
}

H5Group::H5Group(H5Object & _parent, hid_t _group, const char * _name) : H5Object(_parent), group(_group), name(std::string(_name))
{

}

H5Group::H5Group(H5Object & _parent, hid_t _group, const std::string & _name) : H5Object(_parent), group(_group), name(_name)
{

}

H5Group::~H5Group()
{
    if (group >= 0)
    {
        H5Gclose(group);
    }
}

H5LinksList & H5Group::getLinks()
{
    return *new H5LinksList(*this);
}

H5NamedObjectsList<H5SoftLink> & H5Group::getSoftLinks()
{
    return *new H5NamedObjectsList<H5SoftLink>(*this, -1, H5L_TYPE_SOFT, "Soft Link");
}

H5NamedObjectsList<H5ExternalLink> & H5Group::getExternalLinks()
{
    return *new H5NamedObjectsList<H5ExternalLink>(*this, -1, H5L_TYPE_EXTERNAL, "External Link");
}

H5GroupsList & H5Group::getGroups()
{
    return *new H5GroupsList(*this);
}

H5NamedObjectsList<H5Group> & H5Group::getHardGroups()
{
    return *new H5NamedObjectsList<H5Group>(*this, H5O_TYPE_GROUP, H5L_TYPE_HARD, "Group");
}

H5NamedObjectsList<H5Type> & H5Group::getHardTypes()
{
    return *new H5NamedObjectsList<H5Type>(*this, H5O_TYPE_NAMED_DATATYPE, H5L_TYPE_HARD, "Type");
}

H5NamedObjectsList<H5Dataset> & H5Group::getHardDatasets()
{
    return *new H5NamedObjectsList<H5Dataset>(*this, H5O_TYPE_DATASET, H5L_TYPE_HARD, "Dataset");
}

H5DatasetsList & H5Group::getDatasets()
{
    return *new H5DatasetsList(*this);
}

H5TypesList & H5Group::getTypes()
{
    return *new H5TypesList(*this);
}

const unsigned int H5Group::getLinksSize() const
{
    herr_t err;
    H5G_info_t info;

    err = H5Gget_info(group, &info);
    if (err < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot get the links number"));
    }

    return (unsigned int)info.nlinks;
}

std::string H5Group::getCompletePath() const
{
    std::string name = getName();
    if (name == "/")
    {
        return "/";
    }

    return H5Object::getCompletePath();
}

void H5Group::getAccessibleAttribute(const std::string & _name, const int pos, void * pvApiCtx) const
{
    SciErr err;
    std::string lower(_name);

    try
    {
        H5Object & obj = H5Object::getObject(*const_cast<H5Group *>(this), _name);
        obj.createOnScilabStack(pos, pvApiCtx);
        return;
    }
    catch (H5Exception & e) { }

    std::transform(_name.begin(), _name.end(), lower.begin(), tolower);

    if (lower == "attributes")
    {
        const H5AttributesList & attrs = const_cast<H5Group *>(this)->getAttributes();
        attrs.createOnScilabStack(pos, pvApiCtx);

        return;
    }
    else if (lower == "groups")
    {
        const H5GroupsList & groups = const_cast<H5Group *>(this)->getGroups();
        groups.createOnScilabStack(pos, pvApiCtx);

        return;
    }
    else if (lower == "datasets")
    {
        const H5DatasetsList & sets = const_cast<H5Group *>(this)->getDatasets();
        sets.createOnScilabStack(pos, pvApiCtx);

        return;
    }
    else if (lower == "types")
    {
        const H5TypesList & types = const_cast<H5Group *>(this)->getTypes();
        types.createOnScilabStack(pos, pvApiCtx);

        return;
    }
    else if (lower == "links")
    {
        std::vector<std::string> names;
        std::vector<std::string> types;
        std::vector<std::string> linkstype;
        std::vector<const char *> _str;
        H5Object::getLinksInfo(*this, names, types, linkstype);
        _str.reserve(names.size() * 3);

        for (unsigned int i = 0; i < names.size(); i++)
        {
            _str.push_back(names[i].c_str());
        }
        for (unsigned int i = 0; i < names.size(); i++)
        {
            _str.push_back(linkstype[i].c_str());
        }
        for (unsigned int i = 0; i < names.size(); i++)
        {
            _str.push_back(types[i].c_str());
        }

        err = createMatrixOfString(pvApiCtx, pos, names.size(), 3, &(_str[0]));
        if (err.iErr)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot create a column of strings on the stack."));
        }

        return;
    }

    H5Object::getAccessibleAttribute(_name, pos, pvApiCtx);
}

std::string H5Group::ls() const
{
    std::ostringstream os;
    herr_t err;
    OpData opdata;
    opdata.parent = const_cast<H5Group *>(this);
    opdata.os = &os;
    hsize_t idx = 0;

    err = H5Literate(group, H5_INDEX_NAME, H5_ITER_INC, &idx, printLsInfo, &opdata);
    if (err < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot list group contents"));
    }

    return os.str();
}

herr_t H5Group::printLsInfo(hid_t g_id, const char * name, const H5L_info_t * info, void * op_data)
{
    H5O_info_t oinfo;
    herr_t err;
    H5Object * hobj = 0;
    hid_t obj;
    OpData & opdata = *(OpData *)op_data;

    if (obj < 0)
    {
        return (herr_t) - 1;
    }

    switch (info->type)
    {
        case H5L_TYPE_SOFT:
            hobj = new H5SoftLink(*opdata.parent, name);
            break;
        case H5L_TYPE_EXTERNAL:
            hobj = new H5ExternalLink(*opdata.parent, name);
            break;
        case H5L_TYPE_HARD:
            obj = H5Oopen(g_id, name, H5P_DEFAULT);
            err = H5Oget_info(obj, &oinfo);
            H5Oclose(obj);

            if (err < 0)
            {
                return (herr_t) - 1;
            }

            switch (oinfo.type)
            {
                case H5O_TYPE_GROUP:
                    hobj = new H5Group(*opdata.parent, name);
                    break;
                case H5O_TYPE_DATASET:
                    hobj = new H5Dataset(*opdata.parent, name);
                    break;
                case H5O_TYPE_NAMED_DATATYPE:
                    hobj = new H5Type(*opdata.parent, name);
                    break;
                default:
                    return (herr_t) - 1;
            }
            break;
        default:
            return (herr_t) - 1;
    }

    hobj->printLsInfo(*opdata.os);
    delete hobj;

    return (herr_t)0;
}

void H5Group::printLsInfo(std::ostringstream & os) const
{
    std::string str(getName());
    H5Object::getResizedString(str);

    os << str << "Group" << std::endl;
}

std::string H5Group::dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel) const
{
    std::ostringstream os;
    haddr_t addr = this->getAddr();
    std::map<haddr_t, std::string>::iterator it = alreadyVisited.find(addr);
    if (it != alreadyVisited.end())
    {
        os << H5Object::getIndentString(indentLevel) << "GROUP \"" << getName() << "\" {" << std::endl
           << H5Object::getIndentString(indentLevel + 1) << "HARDLINK \"" << it->second << "\"" << std::endl
           << H5Object::getIndentString(indentLevel) << "}" << std::endl;

        return os.str();
    }
    else
    {
        alreadyVisited.insert(std::pair<haddr_t, std::string>(addr, getCompletePath()));
    }

    H5AttributesList & attrs = const_cast<H5Group *>(this)->getAttributes();
    H5NamedObjectsList<H5SoftLink> & softlinks = const_cast<H5Group *>(this)->getSoftLinks();
    H5NamedObjectsList<H5ExternalLink> & externallinks = const_cast<H5Group *>(this)->getExternalLinks();
    H5NamedObjectsList<H5Group> & hardgroups = const_cast<H5Group *>(this)->getHardGroups();
    H5NamedObjectsList<H5Type> & hardtypes = const_cast<H5Group *>(this)->getHardTypes();
    H5NamedObjectsList<H5Dataset> & harddatasets = const_cast<H5Group *>(this)->getHardDatasets();

    os << H5Object::getIndentString(indentLevel) << "GROUP \"" << name << "\" {" << std::endl;
    os << attrs.dump(alreadyVisited, indentLevel + 1);
    os << hardgroups.dump(alreadyVisited, indentLevel + 1);
    os << hardtypes.dump(alreadyVisited, indentLevel + 1);
    os << harddatasets.dump(alreadyVisited, indentLevel + 1);
    os << softlinks.dump(alreadyVisited, indentLevel + 1);
    os << externallinks.dump(alreadyVisited, indentLevel + 1);
    os << H5Object::getIndentString(indentLevel) << "}" << std::endl;

    delete &attrs;
    delete &softlinks;
    delete &externallinks;
    delete &hardgroups;
    delete &hardtypes;
    delete &harddatasets;

    return os.str();
}

std::string H5Group::toString(const unsigned int indentLevel) const
{
    std::ostringstream os;
    std::string indentString = H5Object::getIndentString(indentLevel + 1);
    const H5GroupsList & groups = const_cast<H5Group *>(this)->getGroups();
    const H5DatasetsList & datasets = const_cast<H5Group *>(this)->getDatasets();
    const H5TypesList & types = const_cast<H5Group *>(this)->getTypes();
    const H5AttributesList & attrs = const_cast<H5Group *>(this)->getAttributes();

    os << H5Object::getIndentString(indentLevel) << "HDF5 Group" << std::endl
       << indentString << _("Filename") << ": " << getFile().getFileName() << std::endl
       << indentString << _("Name") << ": " << getName() << std::endl
       << indentString << _("Path") << ": " << getCompletePath() << std::endl
       << indentString << _("Attributes") << ": [1 x " << attrs.getSize() << "]" << std::endl
       << indentString << _("Groups") << ": [1 x " << groups.getSize() << "]" << std::endl
       << indentString << _("Datasets") << ": [1 x " << datasets.getSize() << "]" << std::endl
       << indentString << _("Types") << ": [1 x " << types.getSize() << "]" << std::endl
       << indentString << _("Links") << ": [" << getLinksSize() << " x 3]";

    delete &groups;
    delete &datasets;
    delete &types;
    delete &attrs;

    return os.str();
}

H5Group & H5Group::createGroup(H5Object & parent, const std::string & name)
{
    hid_t obj;
    hid_t lcpl;
    H5Object * loc = 0;

    if (H5Lexists(parent.getH5Id(), name.c_str(), H5P_DEFAULT) > 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("The group already exists: %s."), name.c_str());
    }

    if (parent.isFile())
    {
        loc = &reinterpret_cast<H5File *>(&parent)->getRoot();
    }
    else
    {
        loc = &parent;
    }

    lcpl = H5Pcreate(H5P_LINK_CREATE);
    H5Pset_create_intermediate_group(lcpl, 1);

    obj = H5Gcreate2(loc->getH5Id(), name.c_str(), lcpl, H5P_DEFAULT, H5P_DEFAULT);
    H5Pclose(lcpl);
    if (obj < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot create the group: %s."), name.c_str());
    }

    return *new H5Group(*loc, obj, name);
}
}
