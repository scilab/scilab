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

#ifndef __H5ATTRIBUTES_HXX__
#define __H5ATTRIBUTES_HXX__

#include "H5Object.hxx"
#include "H5Type.hxx"
#include "H5Data.hxx"
#include "H5Dataspace.hxx"
#include "H5File.hxx"

namespace org_modules_hdf5
{
    class H5Attribute : protected H5Object
    {
	hid_t attr;
	const char * name;

    public :
	
	H5Attribute(H5Object & _parent, const hid_t parentId, const unsigned int pos);
	
	~H5Attribute();

	hid_t getH5Id() const { return attr; }
	virtual std::string getName() const { return std::string(name); }
	H5Data & getData();
	H5Type & getDataType();
	H5Dataspace & getSpace();

	virtual std::string dump(const unsigned int indentLevel) const; 
	virtual std::string toString(const unsigned int indentLevel) const; 
    };
}

#endif // __H5ATTRIBUTE_HXX__
