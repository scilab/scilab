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

#include "H5Object.hxx"
#include "H5File.hxx"
#include "H5Group.hxx"
#include "H5Dataset.hxx"
#include "H5Type.hxx"
#include "H5AttributesList.hxx"
#include "H5Attribute.hxx"

namespace org_modules_hdf5
{

H5Object* H5Object::root = NULL;

H5Object::H5Object(H5Object & _parent, const std::string & _name) : parent(_parent), children(std::set<H5Object * >()), locked(false), scilabId(-1), name(_name)
{
    parent.registerChild(this);
}

H5Object::H5Object(H5Object & _parent) : parent(_parent), children(std::set<H5Object * >()), locked(false), scilabId(-1)
{
    parent.registerChild(this);
}

H5Object::~H5Object()
{
    cleanup();
}

void H5Object::cleanup()
{
    locked = true;
    for (std::set<H5Object *>::iterator it = children.begin(); it != children.end(); it++)
    {
        delete *it;
    }
    children.clear();
    locked = false;
    parent.unregisterChild(this);

    H5VariableScope::removeId(scilabId);
}

hid_t H5Object::getH5Id() const
{
    return (hid_t) - 1;
}

hsize_t H5Object::getAttributesNumber() const
{
    H5O_info_t info;
    H5Oget_info(getH5Id(), &info);

    return info.num_attrs;
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
    while (obj != root)
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
        const std::string name = getName();
        const char * _name = name.c_str();
        err = createMatrixOfString(pvApiCtx, pos, 1, 1, &_name);
        if (err.iErr)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot create a string on the stack."));
        }

        return;
    }
    else if (lower == "path")
    {
        const std::string completePath = getCompletePath();
        const char * path = completePath.c_str();
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
    int id = getScilabId();

    if (id == -1)
    {
        id = H5VariableScope::getVariableId(*const_cast<H5Object *>(this));
        const_cast<H5Object *>(this)->setScilabId(id);
    }

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

void H5Object::getNames(const H5Object & obj, std::vector<std::string> & names, FilterType type) const
{
    herr_t err;
    OpDataFilter opdata(&names, type, true);
    hsize_t idx = 0;

    if (type == ATTRIBUTE)
    {
        err = H5Aiterate(obj.getH5Id(), H5_INDEX_NAME, H5_ITER_NATIVE, &idx, H5Object::filterAttributesIterator, &opdata);
    }
    else
    {
        err = H5Literate(obj.getH5Id(), H5_INDEX_NAME, H5_ITER_NATIVE, &idx, H5Object::filterIterator, &opdata);
    }

    if (err < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot list names."));
    }
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
        throw H5Exception(__LINE__, __FILE__, _("Cannot retrieve information about the object"));
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

    if (parent.isFile() && name == "/")
    {
        obj = &reinterpret_cast<H5File *>(&parent)->getRoot();
    }
    else
    {
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
    }

    return *obj;
}

H5Object & H5Object::getObject(H5Object & parent, const std::string & name, const bool isAttr)
{
    hid_t loc = parent.getH5Id();
    H5O_info_t info;
    herr_t err;
    H5Object * obj = 0;

    if (parent.isFile() && name == "/")
    {
        if (isAttr)
        {
            throw H5Exception(__LINE__, __FILE__, _("Invalid name: %s."), name.c_str());
        }

        obj = &reinterpret_cast<H5File *>(&parent)->getRoot();
    }
    else if (isAttr)
    {
        if (H5Aexists(loc, name.c_str()) > 0)
        {
            obj = new H5Attribute(parent, name);
        }
        else
        {
            throw H5Exception(__LINE__, __FILE__, _("Invalid name: %s."), name.c_str());
        }
    }
    else
    {
        if (H5Lexists(loc, name.c_str(), H5P_DEFAULT) <= 0)
        {
            throw H5Exception(__LINE__, __FILE__, _("Invalid name: %s."), name.c_str());
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
        if (name.at(0) != '/')
        {
            return "/" + name;
        }
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
    LinksInfo info(&linksName, &type, &linksType);

    H5Literate(obj.getH5Id(), H5_INDEX_NAME, H5_ITER_INC, &idx, iterateGetInfo, &info);
}

herr_t H5Object::iterateGetInfo(hid_t g_id, const char * name, const H5L_info_t * info, void * op_data)
{
    H5O_info_t oinfo;
    herr_t err;
    LinksInfo & linfo = *(LinksInfo *)op_data;
    hid_t obj;

    linfo.name->push_back(std::string(name));

    switch (info->type)
    {
        case H5L_TYPE_HARD:
            linfo.linkType->push_back("hard");
            break;
        case H5L_TYPE_SOFT:
            linfo.linkType->push_back("soft");
            break;
        case H5L_TYPE_EXTERNAL:
            linfo.linkType->push_back("external");
            break;
        default:
            linfo.linkType->push_back("error");
    }

    obj = H5Oopen(g_id, name, H5P_DEFAULT);
    if (obj < 0)
    {
        if (info->type == H5L_TYPE_HARD)
        {
            return (herr_t) - 1;
        }

        linfo.type->push_back("dangling");

        return (herr_t)0;
    }

    err = H5Oget_info(obj, &oinfo);
    H5Oclose(obj);

    if (err < 0)
    {
        return (herr_t) - 2;
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

herr_t H5Object::filterAttributesIterator(hid_t location_id, const char * attr_name, const H5A_info_t * ainfo, void * op_data)
{
    OpDataFilter & opdata = *(OpDataFilter *)op_data;
    opdata.name->push_back(std::string(attr_name));

    return (herr_t)0;
}

void H5Object::count(const H5Object & obj, OpDataCount & opdata)
{
    hsize_t idx = 0;
    herr_t err = H5Literate(obj.getH5Id(), H5_INDEX_NAME, H5_ITER_INC, &idx, countIterator, &opdata);
    if (err < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot list group contents"));
    }
}

herr_t H5Object::countIterator(hid_t g_id, const char * name, const H5L_info_t * info, void * op_data)
{
    H5O_info_t oinfo;
    herr_t err;
    hid_t obj;
    OpDataCount & opdata = *(OpDataCount *)op_data;

    switch (info->type)
    {
        case H5L_TYPE_SOFT:
            opdata.soft++;
            break;
        case H5L_TYPE_EXTERNAL:
            opdata.external++;
            break;
        case H5L_TYPE_HARD:
            opdata.hard++;
            break;
        default:
            return (herr_t) - 1;
    }

    if (info->type == H5L_TYPE_HARD)
    {
        obj = H5Oopen_by_addr(g_id, info->u.address);
    }
    else if (opdata.followLink)
    {
        obj = H5Oopen(g_id, name, H5P_DEFAULT);
    }
    else
    {
        return (herr_t)0;
    }

    if (obj < 0)
    {
        if (info->type == H5L_TYPE_HARD)
        {
            return (herr_t) - 1;
        }
        opdata.dangling++;
        return (herr_t)0;
    }

    err = H5Oget_info(obj, &oinfo);
    H5Oclose(obj);
    if (err < 0)
    {
        return err;
    }

    switch (oinfo.type)
    {
        case H5O_TYPE_GROUP:
            opdata.group++;
            break;
        case H5O_TYPE_DATASET:
            opdata.dataset++;
            break;
        case H5O_TYPE_NAMED_DATATYPE:
            opdata.type++;
            break;
        default:
            return (herr_t) - 1;
    }

    return (herr_t)0;
}

herr_t H5Object::filterIterator(hid_t g_id, const char * name, const H5L_info_t * info, void * op_data)
{
    H5O_info_t oinfo;
    herr_t err;
    hid_t obj;
    OpDataFilter & opdata = *(OpDataFilter *)op_data;

    switch (opdata.type)
    {
        case HARD:
            if (info->type == H5L_TYPE_HARD)
            {
                opdata.name->push_back(name);
                return (herr_t)0;
            }
            break;
        case SOFT:
            if (info->type == H5L_TYPE_SOFT)
            {
                opdata.name->push_back(name);
                return (herr_t)0;
            }
            break;
        case EXTERNAL:
            if (info->type == H5L_TYPE_EXTERNAL)
            {
                opdata.name->push_back(name);
                return (herr_t)0;
            }
            break;
        default:
            break;
    }

    if (info->type == H5L_TYPE_HARD)
    {
        obj = H5Oopen_by_addr(g_id, info->u.address);
    }
    else
    {
        obj = H5Oopen(g_id, name, H5P_DEFAULT);
    }

    if (obj < 0)
    {
        if (info->type == H5L_TYPE_HARD)
        {
            return (herr_t) - 1;
        }
        if (opdata.type == DANGLING)
        {
            opdata.name->push_back(name);
            return (herr_t)0;
        }
        else
        {
            return (herr_t)0;
        }
    }

    err = H5Oget_info(obj, &oinfo);
    H5Oclose(obj);
    if (err < 0)
    {
        return err;
    }

    switch (opdata.type)
    {
        case GROUP:
            if (oinfo.type == H5O_TYPE_GROUP)
            {
                opdata.name->push_back(name);
                return (herr_t)0;
            }
            break;
        case DATASET:
            if (oinfo.type == H5O_TYPE_DATASET)
            {
                opdata.name->push_back(name);
                return (herr_t)0;
            }
            break;
        case TYPE:
            if (oinfo.type == H5O_TYPE_NAMED_DATATYPE)
            {
                opdata.name->push_back(name);
                return (herr_t)0;
            }
            break;
        default:
            return (herr_t)0;
    }

    return (herr_t)0;
}

herr_t H5Object::filterSoftLinkIterator(hid_t g_id, const char * name, const H5L_info_t * info, void * op_data)
{
    herr_t err;
    OpDataSoftLinkFilter & opdata = *(OpDataSoftLinkFilter *)op_data;

    if (info->type == H5L_TYPE_SOFT && opdata.type == SOFT)
    {
        char * buf = new char[info->u.val_size];

        err = H5Lget_val(g_id, name, buf, info->u.val_size, H5P_DEFAULT);
        if (err < 0)
        {
            delete[] buf;
            return err;
        }

        opdata.name->push_back(name);
        opdata.value->push_back(std::string(buf));

        delete[] buf;
    }
    else if (info->type == H5L_TYPE_EXTERNAL && opdata.type == EXTERNAL)
    {
        char * buf = new char[info->u.val_size];
        const char * filename = 0;
        const char * obj_path = 0;

        err = H5Lget_val(g_id, name, buf, info->u.val_size, H5P_DEFAULT);
        if (err < 0)
        {
            delete[] buf;
            return err;
        }

        err = H5Lunpack_elink_val(buf, info->u.val_size, 0, &filename, &obj_path);
        if (err < 0)
        {
            delete[] buf;
            return err;
        }

        opdata.name->push_back(name);
        opdata.value->push_back(std::string(filename));
        opdata.value->push_back(std::string(obj_path));

        delete[] buf;
    }

    return (herr_t)0;
}
}

