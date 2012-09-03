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

namespace org_modules_hdf5
{

    H5File::H5File(const char * _filename, const char * _path, const bool _readonly) : H5Object(H5Object::getRoot()), filename(strdup(_filename)), path(_path), root(0)
    {
	if (!H5Fis_hdf5(_filename))
	{
	    throw H5Exception(__LINE__, __FILE__, _("Invalid hdf5 file: %s"), _filename);
	}

	file = H5Fopen(_filename, _readonly ? H5F_ACC_RDONLY : H5F_ACC_RDWR, H5P_DEFAULT);

	if (file < 0)
	{
	    throw H5Exception(__LINE__, __FILE__, _("Cannot open the given hdf5 file: %s"), _filename);
	}
    }

    H5File::~H5File()
    {
	if (file >= 0)
	{
	    H5Fclose(file);
	}
	FREE(const_cast<char *>(filename));
    }

    H5Group & H5File::getRootGroup()
    {
	if (!root)
	{
	    root = new H5Group(*this, strdup("/"));
	}
	return *root;
    }

    std::string H5File::getCompletePath() const
    {
	return "";
    }

    std::string H5File::dump(const unsigned int indentLevel) const
    {
	std::ostringstream os;
	H5Group & _root = const_cast<H5File *>(this)->getRootGroup();
	os << H5Object::getIndentString(indentLevel) << "HDF5 \"" << filename << "\" {" << std::endl
	   << _root.dump(indentLevel + 1)
	   << H5Object::getIndentString(indentLevel) << "}" << std::endl;

	return os.str();
    }

    std::string H5File::toString(const unsigned int indentLevel) const
    {
	herr_t err;
	hsize_t size = 0;
	std::ostringstream os;
	unsigned int major = 0, minor = 0, release = 0;
	std::string indentString = H5Object::getIndentString(indentLevel);

	err = H5Fget_filesize(file, &size);
	if (err < 0)
	{
	    throw H5Exception(__LINE__, __FILE__, _("Cannot retrieve file size: %s"), filename); 
	}

	err = H5get_libversion(&major, &minor, &release);
	os << indentString << _("Filename") << ": " << filename << std::endl
	   << indentString << _("HDF5 library version") << ": " << major << "." << minor << "." << release << std::endl
	   << indentString << _("File size") << ": " << size << std::endl
	   << indentString << _("Group name") << ": " << path << std::flush;

	os << std::endl << "DEBUG FILE" << std::endl << dump(0) << std::endl;

	return os.str();
    }
}
