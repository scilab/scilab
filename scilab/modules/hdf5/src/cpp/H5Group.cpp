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

#include "H5Group.hxx"
#include "H5File.hxx"
#include "H5SoftLinksList.hxx"
#include "H5GroupsList.hxx"
#include "H5DatasetsList.hxx"
#include "H5TypesList.hxx"
#include "H5Link.hxx"
#include "H5BasicData.hxx"

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

H5Group::H5Group(H5Object & _parent, const std::string & _name) : H5Object(_parent, _name)
{
    init();
}

H5Group::H5Group(H5Object & _parent, hid_t _group, const std::string & _name) : H5Object(_parent, _name), group(_group)
{

}

H5Group::~H5Group()
{
    if (group >= 0)
    {
        H5Gclose(group);
    }
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

unsigned int H5Group::getLinksSize() const
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

    std::transform(_name.begin(), _name.end(), lower.begin(), tolower);

    if (lower == "attributes")
    {
        std::vector<std::string> names;
        getNames(*this, names, ATTRIBUTE);
        H5BasicData<char>::putStringVectorOnStack(names, (int)names.size(), 1, pos, pvApiCtx);

        return;
    }
    else if (lower == "groups")
    {
        std::vector<std::string> names;
        getNames(*this, names, GROUP);
        H5BasicData<char>::putStringVectorOnStack(names, (int)names.size(), 1, pos, pvApiCtx);

        return;
    }
    else if (lower == "datasets")
    {
        std::vector<std::string> names;
        getNames(*this, names, DATASET);
        H5BasicData<char>::putStringVectorOnStack(names, (int)names.size(), 1, pos, pvApiCtx);

        return;
    }
    else if (lower == "types")
    {
        std::vector<std::string> names;
        getNames(*this, names, TYPE);
        H5BasicData<char>::putStringVectorOnStack(names, (int)names.size(), 1, pos, pvApiCtx);

        return;
    }
    else if (lower == "externals")
    {
        std::vector<std::string> names;
        getNames(*this, names, EXTERNAL);
        H5BasicData<char>::putStringVectorOnStack(names, (int)names.size(), 1, pos, pvApiCtx);

        return;
    }
    else if (lower == "softs")
    {
        std::vector<std::string> names;
        getNames(*this, names, SOFT);
        H5BasicData<char>::putStringVectorOnStack(names, (int)names.size(), 1, pos, pvApiCtx);

        return;
    }
    else if (lower == "danglings")
    {
        std::vector<std::string> names;
        getNames(*this, names, DANGLING);
        H5BasicData<char>::putStringVectorOnStack(names, (int)names.size(), 1, pos, pvApiCtx);

        return;
    }
    else if (lower == "hards")
    {
        std::vector<std::string> names;
        getNames(*this, names, HARD);
        H5BasicData<char>::putStringVectorOnStack(names, (int)names.size(), 1, pos, pvApiCtx);

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

        err = createMatrixOfString(pvApiCtx, pos, (int)names.size(), 3, &(_str[0]));
        if (err.iErr)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot create a column of strings on the stack."));
        }

        return;
    }
    else
    {
        try
        {
            H5Object & obj = H5Object::getObject(*const_cast<H5Group *>(this), _name);
            obj.createOnScilabStack(pos, pvApiCtx);
            return;
        }
        catch (const H5Exception & /*e*/) { }
    }

    H5Object::getAccessibleAttribute(_name, pos, pvApiCtx);
}

void H5Group::ls(std::vector<std::string> & name, std::vector<std::string> & type) const
{
    herr_t err;
    OpDataGetLs opdata(const_cast<H5Group *>(this), &name, &type);
    hsize_t idx = 0;

    err = H5Literate(group, H5_INDEX_NAME, H5_ITER_INC, &idx, getLsInfo, &opdata);
    if (err < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot list group links."));
    }

    idx = 0;
    err = H5Aiterate(group, H5_INDEX_NAME, H5_ITER_INC, &idx, H5Object::getLsAttributes, &opdata);
    if (err < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot list group attributes."));
    }
}

herr_t H5Group::getLsInfo(hid_t g_id, const char * name, const H5L_info_t * info, void * op_data)
{
    H5O_info_t oinfo;
    herr_t err;
    hid_t obj;
    OpDataGetLs & opdata = *(OpDataGetLs *)op_data;

    switch (info->type)
    {
        case H5L_TYPE_SOFT:
            opdata.name->push_back(name);
            opdata.type->push_back("soft");
            break;
        case H5L_TYPE_EXTERNAL:
            opdata.name->push_back(name);
            opdata.type->push_back("external");
            break;
        case H5L_TYPE_HARD:
            obj = H5Oopen_by_addr(g_id, info->u.address);
            if (obj < 0)
            {
                return (herr_t) - 1;
            }

            err = H5Oget_info(obj, &oinfo);
            H5Oclose(obj);
            if (err < 0)
            {
                return (herr_t) - 1;
            }

            switch (oinfo.type)
            {
                case H5O_TYPE_GROUP:
                    opdata.name->push_back(name);
                    opdata.type->push_back("group");
                    break;
                case H5O_TYPE_DATASET:
                    opdata.name->push_back(name);
                    opdata.type->push_back("dataset");
                    break;
                case H5O_TYPE_NAMED_DATATYPE:
                    opdata.name->push_back(name);
                    opdata.type->push_back("type");
                    break;
                default:
                    return (herr_t) - 1;
            }
            break;
        default:
            return (herr_t) - 1;
    }

    return (herr_t)0;
}

std::string H5Group::ls() const
{
    std::ostringstream os;
    herr_t err;
    OpDataPrintLs opdata;
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
    hid_t obj = 0;
    OpDataPrintLs & opdata = *(OpDataPrintLs *)op_data;

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
    OpDataCount opdata(false);
    H5Object::count(*this, opdata);

    os << H5Object::getIndentString(indentLevel) << "HDF5 Group" << std::endl
       << indentString << "Filename" << ": " << getFile().getFileName() << std::endl
       << indentString << "Name" << ": " << getBaseName() << std::endl
       << indentString << "Path" << ": " << getCompletePath() << std::endl
       << indentString << "Attributes" << ": [1 x " << getAttributesNumber() << "]" << std::endl
       << indentString << "Groups" << ": [1 x " << opdata.group << "]" << std::endl
       << indentString << "Datasets" << ": [1 x " << opdata.dataset << "]" << std::endl
       << indentString << "Types" << ": [1 x " << opdata.type << "]" << std::endl
       << indentString << "Externals" << ": [1 x " << opdata.external << "]" << std::endl
       << indentString << "Softs" << ": [1 x " << opdata.soft << "]";

    return os.str();
}

void H5Group::createGroup(H5Object & parent, const std::string & name)
{
    const char * _name = name.c_str();
    createGroup(parent, 1, &_name);
}

void H5Group::createGroup(H5Object & parent, const int size, const char ** names)
{
    hid_t obj;
    hid_t loc = parent.getH5Id();

    for (unsigned int i = 0; i < (unsigned int)size; i++)
    {
        if (H5Lexists(loc, names[i], H5P_DEFAULT) > 0)
        {
            throw H5Exception(__LINE__, __FILE__, _("The group already exists: %s."), names[i]);
        }

        obj = H5Gcreate(loc, names[i], H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        if (obj < 0)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot create the group: %s."), names[i]);
        }
        H5Gclose(obj);
    }
}
}
