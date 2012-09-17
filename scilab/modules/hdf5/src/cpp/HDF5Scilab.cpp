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
    std::string::size_type lastPos = str.find_first_not_of("\n", 0);
    std::string::size_type pos = str.find_first_of("\n", lastPos);

    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        sciprint("%s\n", str.substr(lastPos, pos - lastPos).c_str());
        lastPos = str.find_first_not_of("\n", pos);
        pos = str.find_first_of("\n", lastPos);
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
    catch (H5Exception & e)
    {
        delete file;
        throw;
    }

    delete file;
}

void HDF5Scilab::readData(const std::string & filename, const std::string & name, int pos, void * pvApiCtx)
{
    H5File * file = new H5File(filename, name, "r");
    H5Object & root = file->getRoot();

    try
    {
        readData(root, ".", pos, pvApiCtx);
    }
    catch (H5Exception & e)
    {
        delete file;
        throw;
    }

    delete file;
}

void HDF5Scilab::readData(H5Object & obj, const std::string & name, int pos, void * pvApiCtx)
{
    H5Object * hobj = &obj;
    const bool mustDelete = name != "." || obj.isFile();

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
        if (hobj->checkType(H5O_TYPE_DATASET))
        {
            reinterpret_cast<H5Dataset *>(hobj)->getData().toScilab(pvApiCtx, pos);
        }
        else
        {
            if (mustDelete)
            {
                delete hobj;
            }
            throw H5Exception(__LINE__, __FILE__, _("Invalid object: not a dataset."));
        }
    }
    catch (H5Exception & e)
    {
        if (mustDelete)
        {
            delete hobj;
        }
        throw;
    }

    if (mustDelete)
    {
        delete hobj;
    }
}

void HDF5Scilab::deleteLink(H5Object & parent, const std::string & name)
{
    herr_t err;
    if (H5Lexists(parent.getH5Id(), name.c_str(), H5P_DEFAULT) <= 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("The link doesn't exist: %s."), name.c_str());
    }

    err = H5Ldelete(parent.getH5Id(), name.c_str(), H5P_DEFAULT);
    if (err < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot remove the link: %s."), name.c_str());
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
}
