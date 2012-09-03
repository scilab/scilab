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

#ifndef __H5FILE_HXX__
#define __H5FILE_HXX__

#include "H5Object.hxx"

namespace org_modules_hdf5
{
    class H5Group;

    class H5File : public H5Object
    {
	const char * filename;
	const char * path;
	hid_t file;
	H5Group * root;

    public :
	
	H5File(const char * _filename, const char * _path = "/", const bool _readonly = false);
	
	~H5File();

	H5Group & getRootGroup();
	hid_t getH5Id() const { return file; }
	virtual std::string getCompletePath() const;
	const char * getFileName() const { return filename; }
	const char * getRootPath() const { return path; }

	virtual std::string toString(const unsigned int indentLevel) const;
	virtual std::string dump(const unsigned int indentLevel = 0) const;
    };
}

#endif // __H5FILE_HXX__
