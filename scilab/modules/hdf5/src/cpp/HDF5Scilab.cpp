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

#include "HDF5Scilab.hxx"

namespace org_modules_hdf5
{

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
    const bool mustDelete = path != "." || obj.isFile();

    if (path != ".")
    {
        hobj = &H5Object::getObject(obj, path);
    }
    else if (obj.isFile())
    {
        hobj = &reinterpret_cast<H5File *>(hobj)->getRoot();
    }

    H5Attribute * attr = new H5Attribute(*hobj, attrName);
    attr->getData().toScilab(pvApiCtx, pos);
    delete attr;

    if (mustDelete)
    {
        delete hobj;
    }
}

void HDF5Scilab::readAttributeData(const std::string & filename, const std::string & path, const std::string & attrName, int pos, void * pvApiCtx)
{
    H5File * file = new H5File(filename, path, "r");
    H5Object & root = file->getRoot();

    try
    {
        readAttributeData(root, ".", attrName, pos, pvApiCtx);
    }
    catch (const H5Exception & e)
    {
        delete file;
        throw;
    }

    delete file;
}

void HDF5Scilab::readData(const std::string & filename, const std::string & name, const unsigned int size, const double * start, const double * stride, const double * count, const double * block, int pos, void * pvApiCtx)
{
    H5File * file = new H5File(filename, name, "r");
    H5Object & root = file->getRoot();

    try
    {
        readData(root, ".", size, start, stride, count, block, pos, pvApiCtx);
    }
    catch (const H5Exception & e)
    {
        delete file;
        throw;
    }

    delete file;
}

void HDF5Scilab::readData(H5Object & obj, const std::string & name, const unsigned int size, const double * start, const double * stride, const double * count, const double * block, int pos, void * pvApiCtx)
{
    H5Object * hobj = &obj;
    const bool mustDelete = name != "." || obj.isFile();
    hsize_t * dims = 0;

    if (name != ".")
    {
        hobj = &H5Object::getObject(obj, name);
    }
    else if (obj.isFile())
    {
        hobj = &reinterpret_cast<H5File *>(hobj)->getRoot();
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
                data->toScilab(pvApiCtx, pos);
            }
            catch (const H5Exception & e)
            {
                if (data)
                {
                    delete data;
                }
                delete &space;
                throw;
            }

            if (!data->isReference())
            {
                delete data;
            }
        }
        else
        {
            throw H5Exception(__LINE__, __FILE__, _("Invalid object: not a dataset."));
        }
    }
    catch (const H5Exception & e)
    {
        if (mustDelete)
        {
            delete hobj;
        }
        if (dims)
        {
            delete[] dims;
        }
        throw;
    }

    if (mustDelete)
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
    H5File * _file = new H5File(file, "/", "r+");

    try
    {
        deleteObject(*_file, name);
        delete _file;
    }
    catch (const H5Exception & e)
    {
        delete _file;
    }
}


void HDF5Scilab::deleteObject(H5Object & parent, const std::string & name)
{
    herr_t err;
    hid_t loc = parent.getH5Id();
    std::string _name = name;
    if (name.empty())
    {
        _name = parent.getName();
        if (!parent.isFile())
        {
            loc = parent.getParent().getH5Id();
        }
    }

    if (H5Lexists(loc, _name.c_str(), H5P_DEFAULT) <= 0)
    {
        if (H5Aexists(loc, _name.c_str()) <= 0)
        {
            throw H5Exception(__LINE__, __FILE__, _("The name doesn't exist: %s."), _name.c_str());
        }

        err = H5Adelete(loc, _name.c_str());
        if (err < 0)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot remove the attribute: %s."), _name.c_str());
        }

        return;
    }

    err = H5Ldelete(loc, _name.c_str(), H5P_DEFAULT);
    if (err < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot remove the link: %s."), _name.c_str());
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
            throw H5Exception(__LINE__, __FILE__, _("Cannot create a hard link to an external object: %s."), name.c_str());
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
        createLink(_file->getRoot(), name, destName, hard);
    }
    catch (const H5Exception & e)
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
    catch (const H5Exception & e)
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

    if (src.isFile())
    {
        sobj = &reinterpret_cast<H5File *>(sobj)->getRoot();
    }

    if (dest.isFile())
    {
        dobj = &reinterpret_cast<H5File *>(dobj)->getRoot();
    }

    name = (dlocation.empty() || dlocation == ".") ? sobj->getBaseName() : dlocation;

    if (sobj->isAttribute())
    {
        H5Attribute * attr = reinterpret_cast<H5Attribute *>(sobj);
        try
        {
            attr->copy(*dobj, name);
        }
        catch (const H5Exception & e)
        {
            if (src.isFile())
            {
                delete sobj;
            }

            if (dest.isFile())
            {
                delete dobj;
            }
            throw;
        }
    }
    else
    {
        err = H5Ocopy(sobj->getH5Id(), slocation.empty() ? "." : slocation.c_str(), dobj->getH5Id(), name.c_str(), H5P_DEFAULT, H5P_DEFAULT);
    }

    if (src.isFile())
    {
        delete sobj;
    }

    if (dest.isFile())
    {
        delete dobj;
    }

    if (err < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot copy object."));
    }

    dest.getFile().flush(true);
}

void HDF5Scilab::copy(H5Object & src, const std::string & slocation, const std::string & dfile, const std::string & dlocation)
{
    H5File * dest = new H5File(dfile, dlocation);

    try
    {
        copy(src, slocation, *dest, ".");
        delete dest;
    }
    catch (const H5Exception & e)
    {
        delete dest;
        throw;
    }
}

void HDF5Scilab::copy(const std::string & sfile, const std::string & slocation, H5Object & dest, const std::string & dlocation)
{
    H5File * src = new H5File(sfile, slocation);

    try
    {
        copy(*src, "", dest, dlocation);
        delete src;
    }
    catch (const H5Exception & e)
    {
        delete src;
        throw;
    }
}

void HDF5Scilab::copy(const std::string & sfile, const std::string & slocation, const std::string & dfile, const std::string & dlocation)
{
    H5File * src = new H5File(sfile, slocation);
    H5File * dest;

    try
    {
        dest = new H5File(dfile, dlocation);
    }
    catch (const H5Exception & e)
    {
        delete src;
        throw;
    }

    try
    {
        copy(*src, "", *dest, ".");
        delete src;
        delete dest;
    }
    catch (const H5Exception & e)
    {
        delete src;
        delete dest;
        throw;
    }
}

void HDF5Scilab::ls(H5Object & obj, std::string name, int position, void * pvApiCtx)
{
    std::vector<std::string> _name;
    std::vector<std::string> _type;
    std::vector<const char *> strs;
    H5Object & hobj = (name.empty() || name == ".") ? obj : H5Object::getObject(obj, name);

    hobj.ls(_name, _type);
    strs.reserve(_name.size() * 2);
    for (unsigned int i = 0; i < _name.size(); i++)
    {
        strs.push_back(_name[i].c_str());
    }
    for (unsigned int i = 0; i < _type.size(); i++)
    {
        strs.push_back(_type[i].c_str());
    }

    if (!name.empty() && name != ".")
    {
        delete &hobj;
    }

    H5BasicData<char *>::create(pvApiCtx, position, _name.size(), 2, const_cast<char **>(&(strs[0])), 0, 0);
}

void HDF5Scilab::ls(std::string path, std::string name, int position, void * pvApiCtx)
{
    H5File * file = new H5File(path, "/", "r");

    try
    {
        ls(*file, name, position, pvApiCtx);
    }
    catch (const H5Exception & e)
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
        createGroup(_file->getRoot(), name);
    }
    catch (const H5Exception & e)
    {
        delete _file;
        throw;
    }

    delete _file;
}

void HDF5Scilab::createGroup(H5Object & parent, const std::vector<std::string> & names)
{
    for (unsigned int i = 0; i < names.size(); i++)
    {
        H5Group::createGroup(parent, names[i]);
    }
}

void HDF5Scilab::createGroup(const std::string & file, const std::vector<std::string> & names)
{
    H5File * _file = new H5File(file, "/", "r+");

    try
    {
        createGroup(_file->getRoot(), names);
    }
    catch (const H5Exception & e)
    {
        delete _file;
        throw;
    }

    delete _file;
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
}
