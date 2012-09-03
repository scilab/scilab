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

#ifndef __H5OBJECT_HXX__
#define __H5OBJECT_HXX__

#include <hdf5.h>

#include <iostream>
#include <string>
#include <set>
#include <cstdlib>

extern "C"
{
#include "MALLOC.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "localization.h"
}

#include "H5Exception.hxx"

#define H5_INDENT_LENGTH 3

namespace org_modules_hdf5
{
    class H5AttributesList;
    class H5File;

    class H5Object
    {
	static H5Object & root;

	bool locked;
	H5Object & parent;

	friend class H5AttributesList;
	friend class H5LinkList;
	friend class H5Dataset;

    public :
	
	H5Object(H5Object & _parent);
	virtual ~H5Object();

	virtual hid_t getH5Id() const;
	virtual H5AttributesList & getAttributes();

	virtual H5O_info_t getInfo() const
	    {
		H5O_info_t info;
		H5Oget_info(getH5Id(), &info);
		
		return info;
	    }

	virtual std::string getName() const { return ""; }
	virtual std::string getCompletePath() const;
	virtual std::string dump(const unsigned int indentLevel = 0) const { return ""; }
	virtual std::string toString() const { return toString(0); } 
	virtual std::string toString(const unsigned int indentLevel) const { return ""; } 

	H5Object & getParent() const { return parent; }
	H5File & getFile() const;

	static std::string getIndentString(const unsigned int indentLevel)
	    {
		return std::string((size_t)(H5_INDENT_LENGTH * indentLevel), ' ');
	    }

	static H5Object & getRoot()
	    {
		return root;
	    }

	static H5Object & getObject(H5Object & parent, hid_t obj);

    protected :
	std::set<H5Object *> children;
	void registerChild(H5Object * child) { if (!locked) children.insert(child); }
	void unregisterChild(H5Object * child) { if (!locked) children.erase(child); }

    private :
	H5Object() : parent(*this) { }
    };
}

#undef H5_INDENT_LENGTH
#endif // __H5OBJECT_HXX__
