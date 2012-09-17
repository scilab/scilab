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

#include "HDF5Objects.h"
#include "H5Group.hxx"
#include "H5File.hxx"

extern "C"
{
#include "FileExist.h"
}

namespace org_modules_hdf5
{

void H5File::init()
{
    bool opened;

#if !defined(__HDF5ERROR_PRINT__)
    H5Eset_auto2(H5E_DEFAULT, 0, 0);
#endif

    switch (flags)
    {
        case RDONLY:
            if (!FileExist(const_cast<char *>(filename.c_str())) || H5Fis_hdf5(filename.c_str()) <= 0)
            {
                throw H5Exception(__LINE__, __FILE__, _("Invalid hdf5 file: %s"), filename.c_str());
            }

            file = H5Fopen(filename.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
            if (file < 0)
            {
                throw H5Exception(__LINE__, __FILE__, _("Cannot open the given hdf5 file: %s"), filename.c_str());
            }

            opened = true;
            break;
        case RDWR:
            if (!FileExist(const_cast<char *>(filename.c_str())) || H5Fis_hdf5(filename.c_str()) <= 0)
            {
                throw H5Exception(__LINE__, __FILE__, _("Invalid hdf5 file: %s"), filename.c_str());
            }

            file = H5Fopen(filename.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
            if (file < 0)
            {
                throw H5Exception(__LINE__, __FILE__, _("Cannot open the given hdf5 file: %s"), filename.c_str());
            }

            opened = true;
            break;
        case TRUNC:
            file = H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
            if (file < 0)
            {
                throw H5Exception(__LINE__, __FILE__, _("Cannot create the given hdf5 file: %s"), filename.c_str());
            }

            break;
        case EXCL:
            file = H5Fcreate(filename.c_str(), H5F_ACC_EXCL, H5P_DEFAULT, H5P_DEFAULT);
            if (file < 0)
            {
                throw H5Exception(__LINE__, __FILE__, _("Cannot create the given hdf5 file: %s"), filename.c_str());
            }
            break;
        case APPEND:
            if (FileExist(const_cast<char *>(filename.c_str())))
            {
                if (H5Fis_hdf5(filename.c_str()) > 0)
                {
                    file = H5Fopen(filename.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
                    if (file < 0)
                    {
                        throw H5Exception(__LINE__, __FILE__, _("Cannot open the given hdf5 file: %s"), filename.c_str());
                    }

                    opened = true;
                }
                else
                {
                    throw H5Exception(__LINE__, __FILE__, _("Cannot append the file (not HDF5): %s"), filename.c_str());
                }
            }
            else
            {
                file = H5Fcreate(filename.c_str(), H5F_ACC_EXCL, H5P_DEFAULT, H5P_DEFAULT);
                if (file < 0)
                {
                    throw H5Exception(__LINE__, __FILE__, _("Cannot create the given hdf5 file: %s"), filename.c_str());
                }
            }
    }

    if (opened && path != "/" && H5Lexists(file, path.c_str(), H5P_DEFAULT) <= 0)
    {
        H5Fclose(file);
        throw H5Exception(__LINE__, __FILE__, _("Invalid path: %s"), path.c_str());
    }
}

H5File::H5File(const char * _filename, const char * _path, const char * access) : H5Object(H5Object::getRoot()), filename(std::string(_filename)), path(std::string(_path)), flags(getFlags(std::string(access)))
{
    init();
}

H5File::H5File(const std::string & _filename, const std::string & _path, const std::string & access) : H5Object(H5Object::getRoot()), filename(_filename), path(_path), flags(getFlags(access))
{
    init();
}

H5File::~H5File()
{
    if (file >= 0)
    {
        H5Fclose(file);
    }
}

H5Object & H5File::getRoot()
{
    hid_t obj = H5Oopen(getH5Id(), path.c_str(), H5P_DEFAULT);
    if (obj < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Invalid path: %s"), path.c_str());
    }

    try
    {
        return H5Object::getObject(*this, obj);
    }
    catch (H5Exception & e)
    {
        H5Oclose(obj);
        throw;
    }
}

std::string H5File::getCompletePath() const
{
    return "";
}

hsize_t H5File::getFileSize() const
{
    herr_t err;
    hsize_t size = 0;

    err = H5Fget_filesize(file, &size);
    if (err < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot retrieve file size: %s"), filename.c_str());
    }

    return size;
}

void H5File::getFileHDF5Version(unsigned int * out) const
{
    herr_t err = H5get_libversion(out, out + 1, out + 2);
    if (err < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot retrieve file version: %s"), filename.c_str());
    }
}

void H5File::getAccessibleAttribute(const std::string & _name, const int pos, void * pvApiCtx) const
{
    SciErr err;
    std::string lower(_name);
    std::transform(_name.begin(), _name.end(), lower.begin(), tolower);

    if (lower == "name")
    {
        const char * _filename = filename.c_str();
        err = createMatrixOfString(pvApiCtx, pos, 1, 1, &_filename);
        if (err.iErr)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot create a string on the stack."));
        }

        return;
    }
    else if (lower == "size")
    {
        unsigned int size = (unsigned int)getFileSize();
        err = createMatrixOfUnsignedInteger32(pvApiCtx, pos, 1, 1, &size);
        if (err.iErr)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot create an integer on the stack."));
        }

        return;
    }
    else if (lower == "version")
    {
        unsigned int version[3];
        getFileHDF5Version(version);
        err = createMatrixOfUnsignedInteger32(pvApiCtx, pos, 1, 3, (unsigned int *)version);
        if (err.iErr)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot create an array of integer on the stack."));
        }

        return;
    }
    else if (lower == "root")
    {
        H5Object & root = const_cast<H5File *>(this)->getRoot();
        root.createOnScilabStack(pos, pvApiCtx);

        return;
    }

    throw H5Exception(__LINE__, __FILE__, _("Invalid field %s."), _name.c_str());
}

std::string H5File::dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel) const
{
    std::ostringstream os;
    H5Object & _root = const_cast<H5File *>(this)->getRoot();
    os << H5Object::getIndentString(indentLevel) << "HDF5 \"" << filename << "\" {" << std::endl
       << _root.dump(alreadyVisited, indentLevel + 1)
       << H5Object::getIndentString(indentLevel) << "}" << std::endl;

    delete &_root;

    return os.str();
}

std::string H5File::ls() const
{
    H5Object & _root = const_cast<H5File *>(this)->getRoot();
    std::string str = _root.ls();
    delete &_root;

    return str;
}

std::string H5File::toString(const unsigned int indentLevel) const
{
    herr_t err;
    hsize_t size = 0;
    std::ostringstream os;
    unsigned int major = 0, minor = 0, release = 0;
    std::string indentString = H5Object::getIndentString(indentLevel + 1);

    err = H5Fget_filesize(file, &size);
    if (err < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot retrieve file size: %s"), filename.c_str());
    }

    err = H5get_libversion(&major, &minor, &release);

    os << H5Object::getIndentString(indentLevel) << "HDF5 File" << std::endl
       << indentString << _("Filename") << ": " << filename << std::endl
       << indentString << _("HDF5 library version") << ": " << major << "." << minor << "." << release << std::endl
       << indentString << _("File size") << ": " << size << std::endl
       << indentString << _("Root object") << ": " << path << std::flush;

    return os.str();
}
}
