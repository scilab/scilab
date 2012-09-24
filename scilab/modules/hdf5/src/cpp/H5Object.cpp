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

#include "H5Object.hxx"
#include "H5File.hxx"
#include "H5Group.hxx"
#include "H5Dataset.hxx"
#include "H5Type.hxx"
#include "H5AttributesList.hxx"
#include "H5Attribute.hxx"

namespace org_modules_hdf5
{

H5Object & H5Object::root = *new H5Object();

H5Object::H5Object(H5Object & _parent, const std::string & _name) : parent(_parent), children(std::set<H5Object *>()), locked(false), scilabId(-1), name(_name)
{
    parent.registerChild(this);
}

H5Object::H5Object(H5Object & _parent) : parent(_parent), children(std::set<H5Object *>()), locked(false), scilabId(-1)
{
    parent.registerChild(this);
}

H5Object::~H5Object()
{
    locked = true;
    for (std::set<H5Object *>::iterator it = children.begin(); it != children.end(); it++)
    {
        delete *it;
    }
    locked = false;
    parent.unregisterChild(this);

    H5VariableScope::removeId(scilabId);
}

hid_t H5Object::getH5Id() const
{
    return (hid_t) - 1;
}

/*std::string H5Object::getName() const
  {
  herr_t err;
  hid_t obj = getH5Id();
  ssize_t size;
  char * name = 0;
  std::string ret;

  size = H5Iget_name(obj, 0, 0);
  if (size < 0)
  {
  throw H5Exception(__LINE__, __FILE__, _("Cannot get object name."));
  }

  name = new char[size + 1];
  if (H5Iget_name(obj, name, size + 1) < 0)
  {
  delete[] name;
  throw H5Exception(__LINE__, __FILE__, _("Cannot get object name."));
  }

  ret = std::string(name);
  delete[] name;

  return ret;
  }*/

H5File & H5Object::getFile() const
{
    const H5Object * sobj = this;
    const H5Object * obj = &parent;
    while (obj != &root)
    {
        sobj = obj;
        obj = &(obj->parent);
    }

    return *reinterpret_cast<H5File *>(const_cast<H5Object *>(sobj));
}

void H5Object::getAccessibleAttribute(const std::string & _name, const int pos, void * pvApiCtx) const
{
    SciErr err;
    std::string lower(_name);
    std::transform(_name.begin(), _name.end(), lower.begin(), tolower);

    if (lower == "name")
    {
        const char * name = getName().c_str();
        err = createMatrixOfString(pvApiCtx, pos, 1, 1, &name);
        if (err.iErr)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot create a string on the stack."));
        }

        return;
    }
    else if (lower == "path")
    {
        const char * path = getCompletePath().c_str();
        err = createMatrixOfString(pvApiCtx, pos, 1, 1, &path);
        if (err.iErr)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot create a string on the stack."));
        }

        return;
    }

    throw H5Exception(__LINE__, __FILE__, _("Invalid field: %s"), _name.c_str());
}

void H5Object::createOnScilabStack(int pos, void * pvApiCtx) const
{
    static const char * fields[] = {"H5Object", "_id"};
    int * mlistaddr = 0;
    SciErr err;
    int id = H5VariableScope::getVariableId(*const_cast<H5Object *>(this));
    const_cast<H5Object *>(this)->setScilabId(id);

    err = createMList(pvApiCtx, pos, 2, &mlistaddr);
    if (err.iErr)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot create a mlist on the stack."));
    }

    err = createMatrixOfStringInList(pvApiCtx, pos, mlistaddr, 1, 1, 2, fields);
    if (err.iErr)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot create a mlist on the stack."));
    }

    err = createMatrixOfInteger32InList(pvApiCtx, pos, mlistaddr, 2, 1, 1, &id);
    if (err.iErr)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot create a mlist on the stack."));
    }
}

void H5Object::createInScilabList(int * list, int stackPos, int pos, void * pvApiCtx) const
{
    static const char * fields[] = {"H5Object", "_id"};
    int * mlistaddr = 0;
    SciErr err;
    int id = H5VariableScope::getVariableId(*const_cast<H5Object *>(this));
    const_cast<H5Object *>(this)->setScilabId(id);

    err = createMListInList(pvApiCtx, stackPos, list, pos, 2, &mlistaddr);
    if (err.iErr)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot create a mlist on the stack."));
    }

    err = createMatrixOfStringInList(pvApiCtx, stackPos, mlistaddr, 1, 1, 2, fields);
    if (err.iErr)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot create a mlist on the stack."));
    }

    err = createMatrixOfInteger32InList(pvApiCtx, stackPos, mlistaddr, 2, 1, 1, &id);
    if (err.iErr)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot create a mlist on the stack."));
    }
}

H5AttributesList & H5Object::getAttributes()
{
    return *new H5AttributesList(*this);
}

H5Object & H5Object::getObject(H5Object & parent, hid_t obj)
{
    H5O_info_t info;
    herr_t err = H5Oget_info(obj, &info);
    ssize_t size;
    char * name = 0;
    std::string _name;
    //TODO: voir pr utiliser getName().

    if (err < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot retrieve informations about the object"));
    }

    size = H5Iget_name(obj, 0, 0);
    name = new char[size + 1];
    H5Iget_name(obj, name, size + 1);
    _name = std::string(name);
    delete[] name;

    switch (info.type)
    {
        case H5O_TYPE_GROUP:
            return *new H5Group(parent, obj, _name);
        case H5O_TYPE_DATASET:
            return *new H5Dataset(parent, obj, _name);
        case H5O_TYPE_NAMED_DATATYPE:
            return *new H5Type(parent, obj, _name);
        case H5O_TYPE_UNKNOWN:
        default:
            throw H5Exception(__LINE__, __FILE__, _("Unknown HDF5 object"));
    }
}

H5Object & H5Object::getObject(H5Object & parent, const std::string & name)
{
    hid_t loc = parent.getH5Id();
    H5O_info_t info;
    herr_t err;
    H5Object * obj = 0;

    if (H5Lexists(loc, name.c_str(), H5P_DEFAULT) <= 0)
    {
        if (H5Aexists(loc, name.c_str()) <= 0)
        {
            throw H5Exception(__LINE__, __FILE__, _("Invalid name: %s."), name.c_str());
        }

        return *new H5Attribute(parent, name);
    }

    err = H5Oget_info_by_name(loc, name.c_str(), &info, H5P_DEFAULT);

    if (err < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Invalid name: %s."), name.c_str());
    }

    switch (info.type)
    {
        case H5O_TYPE_GROUP:
            obj = new H5Group(parent, name);
            break;
        case H5O_TYPE_DATASET:
            obj = new H5Dataset(parent, name);
            break;
        case H5O_TYPE_NAMED_DATATYPE:
            obj = new H5Type(parent, name);
            break;
        default:
            throw H5Exception(__LINE__, __FILE__, _("Invalid HDF5 object"));
    }

    return *obj;
}

std::string H5Object::getCompletePath() const
{
    std::string name = getName();
    if (name.empty())
    {
        return parent.getCompletePath();
    }

    std::string path = parent.getCompletePath();
    if (path.empty())
    {
        return name;
    }

    if (path.at(path.length() - 1) == '/' && name.at(0) == '/')
    {
        return path + name.substr(1);
    }
    else if (path.at(path.length() - 1) == '/' || name.at(0) == '/')
    {
        return path + name;
    }

    return path + "/" + name;
}

void H5Object::getLinksInfo(const H5Object & obj, std::vector<std::string> & linksName, std::vector<std::string> & type, std::vector<std::string> & linksType)
{
    hsize_t idx = 0;
    LinksInfo_ info;

    info.name = &linksName;
    info.type = &type;
    info.linktype = &linksType;

    H5Literate(obj.getH5Id(), H5_INDEX_NAME, H5_ITER_INC, &idx, iterateGetInfo, &info);
}

herr_t H5Object::iterateGetInfo(hid_t g_id, const char * name, const H5L_info_t * info, void * op_data)
{
    H5O_info_t oinfo;
    herr_t err;
    LinksInfo_ & linfo = *(LinksInfo_ *)op_data;
    hid_t obj = H5Oopen(g_id, name, H5P_DEFAULT);

    if (obj < 0)
    {
        return (herr_t) - 1;
    }

    err = H5Oget_info(obj, &oinfo);
    H5Oclose(obj);

    if (err < 0)
    {
        return (herr_t) - 2;
    }

    linfo.name->push_back(std::string(name));

    switch (info->type)
    {
        case H5L_TYPE_HARD:
            linfo.linktype->push_back("hard");
            break;
        case H5L_TYPE_SOFT:
            linfo.linktype->push_back("soft");
            break;
        case H5L_TYPE_EXTERNAL:
            linfo.linktype->push_back("external");
            break;
        default:
            linfo.linktype->push_back("error");
    }

    switch (oinfo.type)
    {
        case H5O_TYPE_GROUP:
            linfo.type->push_back("group");
            break;
        case H5O_TYPE_DATASET:
            linfo.type->push_back("dataset");
            break;
        case H5O_TYPE_NAMED_DATATYPE:
            linfo.type->push_back("datatype");
            break;
        default:
            linfo.type->push_back("unknown");
    }

    return (herr_t)0;
}

herr_t H5Object::getLsAttributes(hid_t location_id, const char * attr_name, const H5A_info_t * ainfo, void * op_data)
{
    OpDataGetLs & opdata = *(OpDataGetLs *)op_data;
    opdata.name->push_back(std::string(attr_name));
    opdata.type->push_back("attribute");

    return (herr_t)0;
}
}
