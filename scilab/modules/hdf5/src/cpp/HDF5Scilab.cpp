/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "HDF5Scilab.hxx"

namespace org_modules_hdf5
{

std::map<std::string, H5Object::FilterType> HDF5Scilab::filtersName = initFilterNames();

std::map<std::string, H5Object::FilterType> HDF5Scilab::initFilterNames()
{
    std::map<std::string, H5Object::FilterType> ret;
    ret.insert(std::pair<std::string, H5Object::FilterType>("group", H5Object::GROUP));
    ret.insert(std::pair<std::string, H5Object::FilterType>("g", H5Object::GROUP));
    ret.insert(std::pair<std::string, H5Object::FilterType>("dataset", H5Object::DATASET));
    ret.insert(std::pair<std::string, H5Object::FilterType>("d", H5Object::DATASET));
    ret.insert(std::pair<std::string, H5Object::FilterType>("type", H5Object::TYPE));
    ret.insert(std::pair<std::string, H5Object::FilterType>("t", H5Object::TYPE));
    ret.insert(std::pair<std::string, H5Object::FilterType>("attribute", H5Object::ATTRIBUTE));
    ret.insert(std::pair<std::string, H5Object::FilterType>("a", H5Object::ATTRIBUTE));

    return ret;
}

int HDF5Scilab::getH5ObjectId(int * mlist, void * pvApiCtx)
{
    int * id = 0;
    int row, col;

    SciErr err = getMatrixOfInteger32InList(pvApiCtx, mlist, 2, &row, &col, &id);

    if (err.iErr)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot get H5Object id"));
    }

    return *id;
}

H5Object * HDF5Scilab::getH5Object(int * mlist, void * pvApiCtx)
{
    int id = getH5ObjectId(mlist, pvApiCtx);
    return H5VariableScope::getVariableFromId(id);
}

bool HDF5Scilab::isH5Object(int * mlist, void * pvApiCtx)
{
    char * mlist_type[2];
    int type;
    int rows, cols;
    int lengths[2];

    SciErr err = getVarType(pvApiCtx, mlist, &type);

    if (err.iErr || type != sci_mlist)
    {
        return false;
    }

    err = getMatrixOfStringInList(pvApiCtx, mlist, 1, &rows, &cols, 0, 0);
    if (err.iErr || rows != 1 || cols != 2)
    {
        return false;
    }

    err = getMatrixOfStringInList(pvApiCtx, mlist, 1, &rows, &cols, lengths, 0);
    if (err.iErr)
    {
        return false;
    }

    for (int i = 0; i < 2; i++)
    {
        mlist_type[i] = new char[lengths[i] + 1];
    }

    err = getMatrixOfStringInList(pvApiCtx, mlist, 1, &rows, &cols, lengths, mlist_type);
    if (err.iErr)
    {
        return false;
    }

    bool ret = !strcmp(mlist_type[0], __SCILAB_MLIST_H5OBJECT__) && !strcmp(mlist_type[1], "_id");

    for (int i = 0; i < 2; i++)
    {
        delete[] mlist_type[i];
    }

    return ret;
}

void HDF5Scilab::scilabPrint(const std::string & str)
{
    std::string::size_type lastPos = str.find_first_not_of('\n', 0);
    std::string::size_type pos = str.find_first_of('\n', lastPos);

    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        sciprint("%s\n", str.substr(lastPos, pos - lastPos).c_str());
        lastPos = str.find_first_not_of('\n', pos);
        pos = str.find_first_of('\n', lastPos);
    }
}

void HDF5Scilab::split(const std::string & str, std::vector<std::string> & v, const char c)
{
    std::string::size_type lastPos = str.find_first_not_of(c, 0);
    std::string::size_type pos = str.find_first_of(c, lastPos);

    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        v.push_back(str.substr(lastPos, pos - lastPos));
        lastPos = str.find_first_not_of(c, pos);
        pos = str.find_first_of(c, lastPos);
    }
}

void HDF5Scilab::readAttributeData(H5Object & obj, const std::string & path, const std::string & attrName, int pos, void * pvApiCtx)
{
    H5Object * hobj = &obj;

    if (path != ".")
    {
        hobj = &H5Object::getObject(obj, path);
    }

    H5Attribute * attr = new H5Attribute(*hobj, attrName);
    attr->getData().toScilab(pvApiCtx, pos, 0, 0, H5Options::isReadFlip());

    if (path != ".")
    {
        delete hobj;
    }
    else
    {
        delete attr;
    }
}

void HDF5Scilab::readAttributeData(const std::string & filename, const std::string & path, const std::string & attrName, int pos, void * pvApiCtx)
{
    H5File * file = new H5File(filename, "/", "r");

    try
    {
        readAttributeData(*file, path, attrName, pos, pvApiCtx);
    }
    catch (const H5Exception & /*e*/)
    {
        delete file;
        throw;
    }

    delete file;
}

void HDF5Scilab::readData(const std::string & filename, const std::string & name, const unsigned int size, const double * start, const double * stride, const double * count, const double * block, int pos, void * pvApiCtx)
{
    H5File * file = new H5File(filename, "/", "r");

    try
    {
        readData(*file, name, size, start, stride, count, block, pos, pvApiCtx);
    }
    catch (const H5Exception & /*e*/)
    {
        delete file;
        throw;
    }

    delete file;
}

void HDF5Scilab::readData(H5Object & obj, const std::string & name, const unsigned int size, const double * start, const double * stride, const double * count, const double * block, int pos, void * pvApiCtx)
{
    H5Object * hobj = &obj;
    hsize_t * dims = 0;

    if (!H5Object::isEmptyPath(name))
    {
        hobj = &H5Object::getObject(obj, name);
    }

    try
    {
        if (hobj->isDataset())
        {
            H5Dataset * dataset = reinterpret_cast<H5Dataset *>(hobj);
            H5Dataspace & space = dataset->getSpace();
            H5Data * data = 0;
            try
            {
                dims = space.select(size, start, stride, count, block);
                data = &dataset->getData(space, dims);
                data->toScilab(pvApiCtx, pos, 0, 0, H5Options::isReadFlip());
            }
            catch (const H5Exception & /*e*/)
            {
                if (data)
                {
                    delete data;
                }
                delete &space;
                throw;
            }

            if (data->mustDelete())
            {
                delete data;
            }
            else if (!H5Object::isEmptyPath(name))
            {
                hobj->unregisterChild(data);
            }
        }
        else
        {
            throw H5Exception(__LINE__, __FILE__, _("Invalid object: not a dataset."));
        }
    }
    catch (const H5Exception & /*e*/)
    {
        if (!H5Object::isEmptyPath(name))
        {
            delete hobj;
        }
        if (dims)
        {
            delete[] dims;
        }
        throw;
    }

    if (!H5Object::isEmptyPath(name))
    {
        delete hobj;
    }
    if (dims)
    {
        delete[] dims;
    }
}

void HDF5Scilab::deleteObject(const std::string & file, const std::string & name)
{
    const H5File * _file = new H5File(file, "/", "r+");

    try
    {
        deleteObject(*_file, name);
        delete _file;
    }
    catch (const H5Exception & /*e*/)
    {
        delete _file;
    }
}

void HDF5Scilab::deleteObject(const std::string & file, const int size, const char ** names)
{
    const H5File * _file = new H5File(file, "/", "r+");

    try
    {
        deleteObject(*_file, size, names);
        delete _file;
    }
    catch (const H5Exception & /*e*/)
    {
        delete _file;
    }
}

void HDF5Scilab::deleteObject(const H5Object & parent, const std::string & name)
{
    const char * _name = name.c_str();
    deleteObject(parent, 1, &_name);
}

void HDF5Scilab::deleteObject(const H5Object & parent, const int size, const char ** names)
{
    herr_t err;
    hid_t loc = parent.getH5Id();

    for (unsigned int i = 0; i < (unsigned int)size; i++)
    {
        const char * _name = names[i];

        if (H5Object::isEmptyPath(_name))
        {
            if (parent.isFile())
            {
                throw H5Exception(__LINE__, __FILE__, _("Cannot remove a file."));
            }
            else
            {
                _name = parent.getName().c_str();
                loc = parent.getParent().getH5Id();
            }
        }

        if (_name[0] == '/' && _name[1] == '\0')
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot remove root element."));
        }

        if (H5Lexists(loc, _name, H5P_DEFAULT) <= 0)
        {
            if (H5Aexists(loc, _name) <= 0)
            {
                throw H5Exception(__LINE__, __FILE__, _("The name doesn't exist: %s."), _name);
            }

            err = H5Adelete(loc, _name);
            if (err < 0)
            {
                throw H5Exception(__LINE__, __FILE__, _("Cannot remove the attribute: %s."), _name);
            }

            continue;
        }

        err = H5Ldelete(loc, _name, H5P_DEFAULT);
        if (err < 0)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot remove the link: %s."), _name);
        }
    }
}

void HDF5Scilab::getObject(H5Object & parent, const std::string & location, const bool isAttr, int position, void * pvApiCtx)
{
    H5Object * obj = 0;
    try
    {
        obj = &H5Object::getObject(parent, location, isAttr);
    }
    catch (const H5Exception & /*e*/)
    {
        H5BasicData<double>::create(pvApiCtx, position, 0, 0, (double *)0, 0, 0);
        return;
    }

    try
    {
        obj->createOnScilabStack(position, pvApiCtx);
    }
    catch (const H5Exception & /*e*/)
    {
        delete obj;
        throw;
    }
}

void HDF5Scilab::createLink(H5Object & parent, const std::string & name, const std::string & targetPath, const bool hard)
{
    herr_t err;
    if (H5Lexists(parent.getH5Id(), name.c_str(), H5P_DEFAULT) > 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("The link already exists: %s."), name.c_str());
    }

    if (hard)
    {
        err = H5Lcreate_hard(parent.getFile().getH5Id(), targetPath.c_str(), parent.getH5Id(), name.c_str(), H5P_DEFAULT, H5P_DEFAULT);
    }
    else
    {
        err = H5Lcreate_soft(targetPath.c_str(), parent.getH5Id(), name.c_str(), H5P_DEFAULT, H5P_DEFAULT);
    }

    if (err < 0)
    {
        if (hard)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot create the hard link: %s."), name.c_str());
        }
        else
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot create the soft link: %s."), name.c_str());
        }
    }
}

void HDF5Scilab::createLink(H5Object & parent, const std::string & name, H5Object & targetObject, const bool hard)
{
    if (parent.getFile().getFileName() == targetObject.getFile().getFileName())
    {
        createLink(parent, name, targetObject.getCompletePath(), hard);
    }
    else
    {
        if (hard)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot create a hard link to the external object: %s."), name.c_str());
        }
        createLink(parent, name, targetObject);
    }
}

void HDF5Scilab::createLink(H5Object & parent, const std::string & name, const std::string & targetFile, const std::string & targetPath)
{
    herr_t err;
    if (H5Lexists(parent.getH5Id(), name.c_str(), H5P_DEFAULT) > 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("The link already exists: %s."), name.c_str());
    }

    err = H5Lcreate_external(targetFile.c_str(), targetPath.c_str(), parent.getH5Id(), name.c_str(), H5P_DEFAULT, H5P_DEFAULT);
    if (err < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot create the external link: %s."), name.c_str());
    }
}

void HDF5Scilab::createLink(H5Object & parent, const std::string & name, H5Object & targetObject)
{
    createLink(parent, name, targetObject.getFile().getFileName(), targetObject.getCompletePath());
}

void HDF5Scilab::createLink(const std::string & file, const std::string & location, const std::string & name, const std::string & destName, const bool hard)
{
    H5File * _file = new H5File(file, location, "r+");

    try
    {
        createLink(*_file, name, destName, hard);
    }
    catch (const H5Exception & /*e*/)
    {
        delete _file;
        throw;
    }

    delete _file;
}

void HDF5Scilab::createLink(const std::string & file, const std::string & location, const std::string & name, const std::string & destFile, const std::string & destName)
{
    H5File * _file = new H5File(file, location, "r+");

    try
    {
        createLink(_file->getRoot(), name, destFile, destName);
    }
    catch (const H5Exception & /*e*/)
    {
        delete _file;
        throw;
    }

    delete _file;
}

void HDF5Scilab::copy(H5Object & src, const std::string & slocation, H5Object & dest, const std::string & dlocation)
{
    H5Object * sobj = &src;
    H5Object * dobj = &dest;
    std::string name;
    herr_t err;

    if (H5Object::isEmptyPath(dlocation))
    {
        std::string bname = sobj->getBaseName();
        if (slocation.empty())
        {
            name = bname;
        }
        else
        {
            std::string::size_type pos = slocation.find_last_of('/');
            if (pos == std::string::npos)
            {
                name = slocation;
            }
            else
            {
                name = slocation.substr(pos + 1);
            }
        }
    }
    else
    {
        name = dlocation;
    }

    //src.getFile().flush(true);

    if (sobj->isAttribute())
    {
        H5Attribute * attr = reinterpret_cast<H5Attribute *>(sobj);
        try
        {
            attr->copy(*dobj, name);
            return;
        }
        catch (const H5Exception & /*e*/)
        {
            throw;
        }
    }
    else
    {
        err = H5Ocopy(sobj->getH5Id(), slocation.empty() ? "." : slocation.c_str(), dobj->getH5Id(), name.c_str(), H5P_DEFAULT, H5P_DEFAULT);
    }

    if (err < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot copy object."));
    }
}

void HDF5Scilab::copy(H5Object & src, const std::string & slocation, const std::string & dfile, const std::string & dlocation)
{
    H5File * dest = new H5File(dfile, "/", "r+");

    try
    {
        copy(src, slocation, *dest, dlocation);
        delete dest;
    }
    catch (const H5Exception & /*e*/)
    {
        delete dest;
        throw;
    }
}

void HDF5Scilab::copy(const std::string & sfile, const std::string & slocation, H5Object & dest, const std::string & dlocation)
{
    H5File * src = new H5File(sfile, "/", "r");

    try
    {
        copy(*src, slocation, dest, dlocation);
        delete src;
    }
    catch (const H5Exception & /*e*/)
    {
        delete src;
        throw;
    }
}

void HDF5Scilab::copy(const std::string & sfile, const std::string & slocation, const std::string & dfile, const std::string & dlocation)
{
    H5File * src = new H5File(sfile, "/", "r");
    H5File * dest;

    try
    {
        dest = new H5File(dfile, "/");
    }
    catch (const H5Exception & /*e*/)
    {
        delete src;
        throw;
    }

    try
    {
        copy(*src, slocation, *dest, dlocation);
        delete src;
        delete dest;
    }
    catch (const H5Exception & /*e*/)
    {
        delete src;
        delete dest;
        throw;
    }
}

void HDF5Scilab::ls(H5Object & obj, const std::string & name, int position, void * pvApiCtx)
{
    std::vector<std::string> _name;
    std::vector<std::string> _type;
    std::vector<const char *> strs;
    H5Object & hobj = H5Object::isEmptyPath(name) ? obj : H5Object::getObject(obj, name);

    hobj.ls(_name, _type);

    if (_name.size() == 0)
    {
        H5BasicData<char *>::create(pvApiCtx, position, 0, 0, "", 0, 0);
    }
    else
    {
        strs.reserve(_name.size() * 2);
        for (unsigned int i = 0; i < _name.size(); i++)
        {
            strs.push_back(_name[i].c_str());
        }
        for (unsigned int i = 0; i < _type.size(); i++)
        {
            strs.push_back(_type[i].c_str());
        }

        if (!H5Object::isEmptyPath(name))
        {
            delete &hobj;
        }

        H5BasicData<char *>::create(pvApiCtx, position, (int)_name.size(), 2, const_cast<char **>(&(strs[0])), 0, 0);
    }
}

void HDF5Scilab::ls(const std::string & path, const std::string & name, int position, void * pvApiCtx)
{
    H5File * file = new H5File(path, "/", "r");

    try
    {
        ls(*file, name, position, pvApiCtx);
    }
    catch (const H5Exception & /*e*/)
    {
        delete file;
        throw;
    }

    delete file;
}

void HDF5Scilab::ls(H5Object & obj, const std::string & name, const std::string & type, int position, void * pvApiCtx)
{
    std::vector<std::string> _name;
    std::vector<const char *> strs;
    std::string lower(type);
    std::transform(type.begin(), type.end(), lower.begin(), tolower);
    std::map<std::string, H5Object::FilterType>::iterator it = filtersName.find(lower);
    H5Object::FilterType ftype;
    if (it != filtersName.end())
    {
        ftype = it->second;
    }
    else
    {
        throw H5Exception(__LINE__, __FILE__, _("Invalid filter"));
    }

    H5Object & hobj = H5Object::isEmptyPath(name) ? obj : H5Object::getObject(obj, name);


    hobj.ls(_name, ftype);
    if (_name.size() == 0)
    {
        H5BasicData<char *>::create(pvApiCtx, position, 0, 0, "", 0, 0);
    }
    else
    {
        strs.reserve(_name.size());
        for (unsigned int i = 0; i < _name.size(); i++)
        {
            strs.push_back(_name[i].c_str());
        }

        if (!H5Object::isEmptyPath(name))
        {
            delete &hobj;
        }

        H5BasicData<char *>::create(pvApiCtx, position, (int)_name.size(), 1, const_cast<char **>(&(strs[0])), 0, 0);
    }
}

void HDF5Scilab::ls(const std::string & path, const std::string & name, const std::string & type, int position, void * pvApiCtx)
{
    H5File * file = new H5File(path, "/", "r");

    try
    {
        ls(*file, name, type, position, pvApiCtx);
    }
    catch (const H5Exception & /*e*/)
    {
        delete file;
        throw;
    }

    delete file;
}


void HDF5Scilab::createGroup(H5Object & parent, const std::string & name)
{
    H5Group::createGroup(parent, name);
}

void HDF5Scilab::createGroup(const std::string & file, const std::string & name)
{
    H5File * _file = new H5File(file, "/", "r+");

    try
    {
        createGroup(*_file, name);
    }
    catch (const H5Exception & /*e*/)
    {
        delete _file;
        throw;
    }

    delete _file;
}

void HDF5Scilab::createGroup(H5Object & parent, const int size, const char ** names)
{
    H5Group::createGroup(parent, size, names);
}

void HDF5Scilab::createGroup(const std::string & file, const int size, const char ** names)
{
    H5File * _file = new H5File(file, "/", "r+");

    try
    {
        createGroup(*_file, size, names);
    }
    catch (const H5Exception & /*e*/)
    {
        delete _file;
        throw;
    }

    delete _file;
}

void HDF5Scilab::label(const std::string & filename, const std::string & location, const unsigned int size, const unsigned int * dim, const char ** names)
{
    H5File * file = new H5File(filename, "/", "r+");

    try
    {
        label(*file, location, size, dim, names);
    }
    catch (const H5Exception & /*e*/)
    {
        delete file;
        throw;
    }

    delete file;
}

void HDF5Scilab::label(H5Object & obj, const std::string & location, const unsigned int size, const unsigned int * dim, const char ** names)
{
    H5Object & hobj = H5Object::isEmptyPath(location) ? obj : H5Object::getObject(obj, location);
    if (hobj.isDataset())
    {
        try
        {
            reinterpret_cast<H5Dataset *>(&hobj)->label(size, dim, names);
        }
        catch (const H5Exception & /*e*/)
        {
            if (!H5Object::isEmptyPath(location))
            {
                delete &hobj;
            }
        }
    }
    else
    {
        if (!H5Object::isEmptyPath(location))
        {
            delete &hobj;
        }
        throw H5Exception(__LINE__, __FILE__, _("Can only label a dataset"));
    }

    if (!H5Object::isEmptyPath(location))
    {
        delete &hobj;
    }
}

int * HDF5Scilab::exists(H5Object & obj, const unsigned int size, const char ** locations, const char ** attrNames)
{
    const hid_t loc = obj.getH5Id();
    const bool isfile = obj.isFile();
    int * res = new int[size];

    if (attrNames)
    {
        if ((isfile && (!strcmp(*locations, "/") || !strcmp(*locations, ".") || **locations == '\0')) || H5Lexists(loc, *locations, H5P_DEFAULT) > 0)
        {
            const hid_t _loc = H5Oopen(loc, *locations, H5P_DEFAULT);
            if (_loc < 0)
            {
                memset(res, 0, sizeof(int) * size);
                return res;
            }

            for (unsigned int i = 0; i < size; i++)
            {
                res[i] = H5Aexists(_loc, attrNames[i]) > 0 ? 1 : 0;
            }
            H5Oclose(_loc);
        }
    }
    else
    {
        for (unsigned int i = 0; i < size; i++)
        {
            res[i] = ((isfile && (!strcmp(locations[i], "/") || !strcmp(locations[i], ".") || *(locations[i]) == '\0')) || H5Lexists(loc, locations[i], H5P_DEFAULT) > 0) ? 1 : 0;
        }
    }

    return res;
}

int * HDF5Scilab::exists(const std::string & filename, const unsigned int size, const char ** locations, const char ** attrNames)
{
    H5File * file = new H5File(filename, "/", "r");
    int * ret = exists(*file, size, locations, attrNames);
    delete file;

    return ret;
}

bool HDF5Scilab::checkType(const H5Object & obj, const H5ObjectType type)
{
    switch (type)
    {
        case H5FILE:
            return obj.isFile();
        case H5GROUP:
            return obj.isGroup();
        case H5DATASET:
            return obj.isDataset();
        case H5ATTRIBUTE:
            return obj.isAttribute();
        case H5SPACE:
            return obj.isDataspace();
        case H5TYPE:
            return obj.isType();
        case H5REFERENCE:
            return obj.isReference();
        case H5LIST:
            return obj.isList();
        case H5COMPOUND:
            return obj.isCompound();
        case H5ARRAY:
            return obj.isArray();
        case H5VLEN:
            return obj.isVlen();
        default:
            return false;
    }
}

void HDF5Scilab::mount(H5Object & obj, const std::string & location, H5Object & file)
{
    herr_t err;

    if (!file.isFile())
    {
        throw H5Exception(__LINE__, __FILE__, _("Target object is not a file"));
    }

    if (location.empty())
    {
        throw H5Exception(__LINE__, __FILE__, _("Invalid location"));
    }

    if (H5Lexists(obj.getH5Id(), location.c_str(), H5P_DEFAULT) <= 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Invalid location: %s"), location.c_str());
    }

    err = H5Fmount(obj.getH5Id(), location.c_str(), file.getH5Id(), H5P_DEFAULT);
    if (err < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot mount the file: %s"), file.getFile().getFileName().c_str());
    }
}

void HDF5Scilab::umount(H5Object & obj, const std::string & location)
{
    herr_t err;
    if (location.empty())
    {
        throw H5Exception(__LINE__, __FILE__, _("Invalid location"));
    }

    err = H5Funmount(obj.getH5Id(), location.c_str());
    if (err < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot unmount the file at location: %s"), location.c_str());
    }
}

void HDF5Scilab::getScilabData(hid_t * type, unsigned int * ndims, hsize_t ** dims, void ** data, bool * mustDelete, bool * mustDeleteContent, const bool flip, int rhsPosition, void * pvApiCtx)
{
    SciErr err;
    int * addr = 0;

    err = getVarAddressFromPosition(pvApiCtx, rhsPosition, &addr);
    if (err.iErr)
    {
        throw H5Exception(__LINE__, __FILE__, _("Can not read input argument #%d."), rhsPosition);
    }

    getScilabData(type, ndims, dims, data, mustDelete, mustDeleteContent, flip, addr, rhsPosition, pvApiCtx);
}

void HDF5Scilab::getScilabData(hid_t * type, unsigned int * ndims, hsize_t ** dims, void ** data, bool * mustDelete, bool * mustDeleteContent, const bool flip, int * addr, int rhsPosition, void * pvApiCtx)
{
    SciErr err;
    int row;
    int col;
    int _type;

    *mustDelete = false;
    *mustDeleteContent = false;

    err = getVarType(pvApiCtx, addr, &_type);
    if (err.iErr)
    {
        throw H5Exception(__LINE__, __FILE__, _("Can not get the type of input argument #%d."), rhsPosition);
    }

    switch (_type)
    {
        case sci_matrix :
        {
            if (isVarComplex(pvApiCtx, addr))
            {
                doublecomplex * mat = 0;
                double * re = 0;
                double * im = 0;
                hid_t complex_id = H5Tcreate(H5T_COMPOUND, sizeof(doublecomplex));
                H5Tinsert(complex_id, "real", offsetof(doublecomplex, r), H5T_NATIVE_DOUBLE);
                H5Tinsert(complex_id, "imag", offsetof(doublecomplex, i), H5T_NATIVE_DOUBLE);

                err = getComplexMatrixOfDouble(pvApiCtx, addr, &row, &col, &re, &im);
                if (err.iErr)
                {
                    H5Tclose(complex_id);
                    throw H5Exception(__LINE__, __FILE__, _("%s: Can not read input argument #%d."), rhsPosition);
                }

                mat = new doublecomplex[row * col];
                for (int i = 0; i < row * col; i++)
                {
                    mat[i].r = re[i];
                    mat[i].i = im[i];
                }

                *type = complex_id;
                *ndims = 2;
                *dims = new hsize_t[*ndims];
                (*dims)[0] = flip ? col : row;
                (*dims)[1] = flip ? row : col;
                *data = mat;
                *mustDelete = true;
            }
            else
            {
                double * mat = 0;
                err = getMatrixOfDouble(pvApiCtx, addr, &row, &col, &mat);
                if (err.iErr)
                {
                    throw H5Exception(__LINE__, __FILE__, _("%s: Can not read input argument #%d."), rhsPosition);
                }
                *type = H5Type::getBaseType(mat);
                *ndims = 2;
                *dims = new hsize_t[*ndims];
                (*dims)[0] = flip ? col : row;
                (*dims)[1] = flip ? row : col;
                *data = mat;
            }
            break;
        }
        case sci_ints :
        {
            int prec = 0;
            void * ints = 0;

            err = getMatrixOfIntegerPrecision(pvApiCtx, addr, &prec);
            if (err.iErr)
            {
                throw H5Exception(__LINE__, __FILE__, _("%s: Can not read input argument #%d."), rhsPosition);
            }

            switch (prec)
            {
                case SCI_INT8 :
                    err = getMatrixOfInteger8(pvApiCtx, addr, &row, &col, (char **)(&ints));
                    if (err.iErr)
                    {
                        throw H5Exception(__LINE__, __FILE__, _("%s: Can not read input argument #%d."), rhsPosition);
                    }
                    *type = H5Type::getBaseType((char *)ints);
                    break;
                case SCI_UINT8 :
                    err = getMatrixOfUnsignedInteger8(pvApiCtx, addr, &row, &col, (unsigned char **)(&ints));
                    if (err.iErr)
                    {
                        throw H5Exception(__LINE__, __FILE__, _("%s: Can not read input argument #%d."), rhsPosition);
                    }
                    *type = H5Type::getBaseType((unsigned char *)ints);
                    break;
                case SCI_INT16 :
                    err = getMatrixOfInteger16(pvApiCtx, addr, &row, &col, (short **)(&ints));
                    if (err.iErr)
                    {
                        throw H5Exception(__LINE__, __FILE__, _("%s: Can not read input argument #%d."), rhsPosition);
                    }
                    *type = H5Type::getBaseType((short *)ints);
                    break;
                case SCI_UINT16 :
                    err = getMatrixOfUnsignedInteger16(pvApiCtx, addr, &row, &col, (unsigned short **)(&ints));
                    if (err.iErr)
                    {
                        throw H5Exception(__LINE__, __FILE__, _("%s: Can not read input argument #%d."), rhsPosition);
                    }
                    *type = H5Type::getBaseType((unsigned short *)ints);
                    break;
                case SCI_INT32 :
                    err = getMatrixOfInteger32(pvApiCtx, addr, &row, &col, (int**)(&ints));
                    if (err.iErr)
                    {
                        throw H5Exception(__LINE__, __FILE__, _("%s: Can not read input argument #%d."), rhsPosition);
                    }
                    *type = H5Type::getBaseType((int *)ints);
                    break;
                case SCI_UINT32 :
                    err = getMatrixOfUnsignedInteger32(pvApiCtx, addr, &row, &col, (unsigned int **)(&ints));
                    if (err.iErr)
                    {
                        throw H5Exception(__LINE__, __FILE__, _("%s: Can not read input argument #%d."), rhsPosition);
                    }
                    *type = H5Type::getBaseType((unsigned int *)ints);
                    break;

#ifdef __SCILAB_INT64__
                case SCI_INT64 :
                    err = getMatrixOfInteger64(pvApiCtx, addr, &row, &col, (long long **)(&ints));
                    if (err.iErr)
                    {
                        throw H5Exception(__LINE__, __FILE__, _("%s: Can not read input argument #%d."), rhsPosition);
                    }
                    *type = H5Type::getBaseType((long long *)ints);
                    break;
                case SCI_UINT64 :
                    err = getMatrixOfUnsignedInteger64(pvApiCtx, addr, &row, &col, (unsigned long long **)(&ints));
                    if (err.iErr)
                    {
                        throw H5Exception(__LINE__, __FILE__, _("%s: Can not read input argument #%d."), rhsPosition);
                    }
                    *type = H5Type::getBaseType((unsigned long long *)ints);
                    break;
#endif
            }

            *ndims = 2;
            *dims = new hsize_t[*ndims];
            (*dims)[0] = flip ? col : row;
            (*dims)[1] = flip ? row : col;
            *data = ints;
            break;
        }
        case sci_strings :
        {
            char ** matS = 0;
            if (getAllocatedMatrixOfString(pvApiCtx, addr, &row, &col, &matS))
            {
                throw H5Exception(__LINE__, __FILE__, _("%s: Can not read input argument #%d."), rhsPosition);
            }
            *type = H5Type::getBaseType((char **)matS);
            *ndims = 2;
            *dims = new hsize_t[*ndims];
            (*dims)[0] = flip ? col : row;
            (*dims)[1] = flip ? row : col;
            *data = matS;
            *mustDelete = true;
            *mustDeleteContent = true;
            break;
        }
        case sci_boolean :
        {
            int * matB;

            err = getMatrixOfBoolean(pvApiCtx, addr, &row, &col, &matB);
            if (err.iErr)
            {
                throw H5Exception(__LINE__, __FILE__, _("%s: Can not read input argument #%d."), rhsPosition);
            }
            *type = H5Type::getBaseType((int *)matB);
            *ndims = 2;
            *dims = new hsize_t[*ndims];
            (*dims)[0] = flip ? col : row;
            (*dims)[1] = flip ? row : col;
            *data = matB;
            break;
        }
        case sci_mlist :
        {
            if (isHypermatType(pvApiCtx, addr))
            {
                int * entries = 0;
                int * _dims = 0;
                int _ndims;

                err = getHypermatEntries(pvApiCtx, addr, &entries);
                if (err.iErr)
                {
                    throw H5Exception(__LINE__, __FILE__, _("%s: Can not read input argument #%d."), rhsPosition);
                }

                getScilabData(type, ndims, dims, data, mustDelete, mustDeleteContent, flip, entries, rhsPosition, pvApiCtx);
                if (*dims)
                {
                    delete[] *dims;
                }

                err = getHypermatDimensions(pvApiCtx, addr, &_dims, &_ndims);
                if (err.iErr)
                {
                    throw H5Exception(__LINE__, __FILE__, _("%s: Can not read input argument #%d."), rhsPosition);
                }

                *dims = new hsize_t[_ndims];
                if (flip)
                {
                    for (int i = 0; i < _ndims; i++)
                    {
                        (*dims)[i] = _dims[_ndims - 1 - i];
                    }
                }
                else
                {
                    for (int i = 0; i < _ndims; i++)
                    {
                        (*dims)[i] = _dims[i];
                    }
                }
                *ndims = _ndims;
            }
            else
            {
                throw H5Exception(__LINE__, __FILE__, _("%s: Datatype not handled for now."));
            }
            break;
        }
        default :
        {
            throw H5Exception(__LINE__, __FILE__, _("%s: Datatype not handled for now."));
        }
    }
}
}
