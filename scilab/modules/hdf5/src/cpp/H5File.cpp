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

#include "HDF5Objects.h"
#include "H5Group.hxx"
#include "H5File.hxx"

extern "C"
{
#include "FileExist.h"
}

namespace org_modules_hdf5
{

void H5File::init(const hid_t fapl)
{
    bool opened = false;

#if !defined(__HDF5ERROR_PRINT__)
    H5Eset_auto(H5E_DEFAULT, 0, 0);
#endif

    if (filename.empty())
    {
        throw H5Exception(__LINE__, __FILE__, _("Invalid hdf5 file: empty filename."));
    }

    switch (flags)
    {
        case RDONLY:
            if (!FileExist(const_cast<char *>(filename.c_str())) || H5Fis_hdf5(filename.c_str()) <= 0)
            {
                throw H5Exception(__LINE__, __FILE__, _("Invalid hdf5 file: %s."), filename.c_str());
            }

            file = H5Fopen(filename.c_str(), H5F_ACC_RDONLY, fapl);
            if (file < 0)
            {
                throw H5Exception(__LINE__, __FILE__, _("Cannot open the given hdf5 file: %s."), filename.c_str());
            }

            opened = true;
            break;
        case RDWR:
            if (!FileExist(const_cast<char *>(filename.c_str())) || H5Fis_hdf5(filename.c_str()) <= 0)
            {
                throw H5Exception(__LINE__, __FILE__, _("Invalid hdf5 file: %s."), filename.c_str());
            }

            file = H5Fopen(filename.c_str(), H5F_ACC_RDWR, fapl);
            if (file < 0)
            {
                throw H5Exception(__LINE__, __FILE__, _("Cannot open the given hdf5 file: %s."), filename.c_str());
            }

            opened = true;
            break;
        case TRUNC:
            file = H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, fapl);
            if (file < 0)
            {
                throw H5Exception(__LINE__, __FILE__, _("Cannot create the given hdf5 file: %s."), filename.c_str());
            }

            break;
        case EXCL:
            file = H5Fcreate(filename.c_str(), H5F_ACC_EXCL, H5P_DEFAULT, fapl);
            if (file < 0)
            {
                throw H5Exception(__LINE__, __FILE__, _("Cannot create the given hdf5 file: %s."), filename.c_str());
            }
            break;
        case APPEND:
            if (FileExist(const_cast<char *>(filename.c_str())))
            {
                if (H5Fis_hdf5(filename.c_str()) > 0)
                {
                    file = H5Fopen(filename.c_str(), H5F_ACC_RDWR, fapl);
                    if (file < 0)
                    {
                        throw H5Exception(__LINE__, __FILE__, _("Cannot open the given hdf5 file: %s."), filename.c_str());
                    }

                    opened = true;
                }
                else
                {
                    struct stat stat_buf;
                    int rc = stat(filename.c_str(), &stat_buf);
                    if (!rc && stat_buf.st_size == 0)
                    {
                        throw H5Exception(__LINE__, __FILE__, _("Cannot open the file: %s, an empty file with the same name already exists."), filename.c_str());
                    }

                    throw H5Exception(__LINE__, __FILE__, _("Cannot append the file (not HDF5): %s."), filename.c_str());
                }
            }
            else
            {
                file = H5Fcreate(filename.c_str(), H5F_ACC_EXCL, H5P_DEFAULT, fapl);
                if (file < 0)
                {
                    throw H5Exception(__LINE__, __FILE__, _("Cannot create the given hdf5 file: %s."), filename.c_str());
                }
            }
            break;
        default:
            throw H5Exception(__LINE__, __FILE__, _("Invalid flag."));
    }

    if (opened && path != "/" && H5Lexists(file, path.c_str(), H5P_DEFAULT) <= 0)
    {
        H5Fclose(file);
        throw H5Exception(__LINE__, __FILE__, _("Invalid path: %s"), path.c_str());
    }
}

H5File::H5File(const std::string & _filename, const std::string & _path, const std::string & access) : H5Object(H5Object::getRoot()), filename(_filename), path(_path), flags(getFlags(access))
{
    hid_t fapl = H5Pcreate(H5P_FILE_ACCESS);
    H5Pset_fclose_degree(fapl, H5F_CLOSE_STRONG);

    try
    {
        init(fapl);
        H5Pclose(fapl);
    }
    catch (const H5Exception & /*e*/)
    {
        H5Pclose(fapl);
        throw;
    }
}

H5File::H5File(const std::string & _filename, const std::string & _path, const std::string & access, const std::string & driver) : H5Object(H5Object::getRoot()), filename(_filename), path(_path), flags(getFlags(access))
{
    hid_t fapl = H5Pcreate(H5P_FILE_ACCESS);
    H5Pset_fclose_degree(fapl, H5F_CLOSE_STRONG);

    if (driver == "stdio")
    {
        H5Pset_fapl_stdio(fapl);
    }
    else if (driver == "sec2")
    {
        H5Pset_fapl_sec2(fapl);
    }

    try
    {
        init(fapl);
        H5Pclose(fapl);
    }
    catch (const H5Exception & /*e*/)
    {
        H5Pclose(fapl);
        throw;
    }
}

H5File::H5File(const std::string & _filename, const std::string & _path, const std::string & access, const bool backingStore, const size_t blockSize) : H5Object(H5Object::getRoot()), filename(_filename), path(_path), flags(getFlags(access))
{
    herr_t err;
    hid_t fapl = H5Pcreate(H5P_FILE_ACCESS);
    H5Pset_fclose_degree(fapl, H5F_CLOSE_STRONG);

    err = H5Pset_fapl_core(fapl, blockSize, (hbool_t)backingStore);
    if (err < 0)
    {
        H5Pclose(fapl);
        throw H5Exception(__LINE__, __FILE__, _("Cannot set \'core\' as driver."));
    }

    try
    {
        init(fapl);
        H5Pclose(fapl);
    }
    catch (const H5Exception & /*e*/)
    {
        H5Pclose(fapl);
        throw;
    }
}

H5File::H5File(const std::string & _filename, const std::string & _path, const std::string & access, const hsize_t memberSize) : H5Object(H5Object::getRoot()), filename(_filename), path(_path), flags(getFlags(access))
{
    herr_t err;
    hid_t fapl;
    hid_t fapl2;

    if (filename.find("%d") == std::string::npos)
    {
        throw H5Exception(__LINE__, __FILE__, _("Invalid filename: must contain a '%d'."));
    }

    fapl = H5Pcreate(H5P_FILE_ACCESS);
    H5Pset_fclose_degree(fapl, H5F_CLOSE_STRONG);
    fapl2 = H5Pcreate(H5P_FILE_ACCESS);
    H5Pset_fclose_degree(fapl2, H5F_CLOSE_STRONG);

    err = H5Pset_fapl_family(fapl, memberSize, fapl2);
    H5Pclose(fapl2);
    if (err < 0)
    {
        H5Pclose(fapl);
        throw H5Exception(__LINE__, __FILE__, _("Cannot set \'family\' as driver."));
    }

    try
    {
        init(fapl);
        H5Pclose(fapl);
    }
    catch (const H5Exception & /*e*/)
    {
        H5Pclose(fapl);
        throw;
    }
}


H5File::~H5File()
{
    H5Fflush(file, H5F_SCOPE_GLOBAL);
    cleanup();
    if (file >= 0)
    {

#if defined(__HDF5OBJECTS_DEBUG__)

        std::cout << "File " << filename << " is closing." << std::endl
                  << "Open groups: " << H5Fget_obj_count(file, H5F_OBJ_GROUP) << std::endl
                  << "Open datasets: " << H5Fget_obj_count(file, H5F_OBJ_DATASET) << std::endl
                  << "Open datatypes: " << H5Fget_obj_count(file, H5F_OBJ_DATATYPE) << std::endl
                  << "Open attributes: " << H5Fget_obj_count(file, H5F_OBJ_ATTR) << std::endl
                  << "Open all (except the file itself): " << H5Fget_obj_count(file, H5F_OBJ_ALL) - 1 << std::endl;

#endif

        H5Fclose(file);
        H5garbage_collect();
    }
}

void H5File::flush(const bool local) const
{
    herr_t err = H5Fflush(file, local ? H5F_SCOPE_LOCAL : H5F_SCOPE_GLOBAL);
    if (err < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Error in flushing the file."));
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
    catch (const H5Exception & /*e*/)
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

    /*    try
        {
    	H5Object & _root = const_cast<H5File *>(this)->getRoot();
            H5Object & obj = H5Object::getObject(_root, _name);
            obj.createOnScilabStack(pos, pvApiCtx);
            return;
        }
        catch (const H5Exception & e) { }
    */

    if (H5Object::isEmptyPath(_name))
    {
        this->createOnScilabStack(pos, pvApiCtx);
        return;
    }

    if (_name.at(0) == '/')
    {
        H5Object * obj = 0;

        try
        {
            obj = &H5Object::getObject(*const_cast<H5File *>(this), _name);
            obj->createOnScilabStack(pos, pvApiCtx);
            return;
        }
        catch (const H5Exception & /*e*/)
        {
            if (obj)
            {
                delete obj;
            }
            throw;
        }
    }

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
        H5Object & _root = const_cast<H5File *>(this)->getRoot();
        _root.createOnScilabStack(pos, pvApiCtx);
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

void H5File::ls(std::vector<std::string> & name, std::vector<std::string> & type) const
{
    H5Object & _root = const_cast<H5File *>(this)->getRoot();
    _root.ls(name, type);
    delete &_root;
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
       << indentString << "Filename" << ": " << filename << std::endl
       << indentString << "Version" << ": " << major << "." << minor << "." << release << std::endl
       << indentString << "Size" << ": " << size << std::endl
       << indentString << "Root" << ": /" <<  std::endl;

    return os.str();
}
}
