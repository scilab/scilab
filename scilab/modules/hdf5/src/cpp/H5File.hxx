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

#ifndef __H5FILE_HXX__
#define __H5FILE_HXX__

#include "H5Object.hxx"

#include <sys/stat.h>

namespace org_modules_hdf5
{
class H5Group;

class H5File : public H5Object
{
    enum FileAccess { RDONLY, RDWR, TRUNC, EXCL, APPEND };

    const std::string filename;
    const std::string path;
    const FileAccess flags;
    hid_t file;

public :

    H5File(const std::string & _filename, const std::string & _path, const std::string & access = "a");
    H5File(const std::string & _filename, const std::string & _path, const std::string & access, const std::string & driver);
    H5File(const std::string & _filename, const std::string & _path, const std::string & access, const bool backingStore, const size_t blockSize);
    H5File(const std::string & _filename, const std::string & _path, const std::string & access, const hsize_t memberSize);

    ~H5File();

    virtual H5Object & getRoot();
    virtual hid_t getH5Id() const
    {
        return file;
    }

    virtual bool isFile() const
    {
        return true;
    }

    void flush(const bool local) const;

    virtual std::string getCompletePath() const;

    const std::string & getFileName() const
    {
        return filename;
    }

    const std::string & getRootPath() const
    {
        return path;
    }

    hsize_t getFileSize() const;
    void getFileHDF5Version(unsigned int * out) const;

    virtual void getAccessibleAttribute(const std::string & name, const int pos, void * pvApiCtx) const;

    virtual std::string toString(const unsigned int indentLevel) const;
    virtual std::string dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel = 0) const;
    virtual std::string ls() const;
    virtual void ls(std::vector<std::string> & name, std::vector<std::string> & type) const;

    static bool checkFileAccess(const std::string & access)
    {
        return access == "r" || access == "r+" || access == "w" || access == "w-" || access == "a";
    }

private :

    void init(const hid_t fapl);

    static FileAccess getFlags(const std::string & access)
    {
        if (access == "r")
        {
            return RDONLY;
        }
        else if (access == "r+")
        {
            return RDWR;
        }
        else if (access == "w")
        {
            return TRUNC;
        }
        else if (access == "w-")
        {
            return EXCL;
        }

        return APPEND;
    }

};
}

#endif // __H5FILE_HXX__
