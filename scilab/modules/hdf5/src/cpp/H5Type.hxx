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

#ifndef __H5TYPE_HXX__
#define __H5TYPE_HXX__

#include "H5Object.hxx"
#include "H5File.hxx"

namespace org_modules_hdf5
{
    
    class H5Type : public H5Object
    {
	hid_t type;
	const char * name;

    public:

	H5Type(H5Object & _parent, hid_t type);
	H5Type(H5Object & _parent, const char * name);
	H5Type(H5Object & _parent, hid_t type, const char * name);
	virtual ~H5Type();

	virtual hid_t getH5Id() const { return type; }
	virtual std::string getName() const { return std::string(name); }
	std::string getClassName() const;
	std::string getTypeName() const;

	virtual std::string dump(const unsigned int indentLevel) const;
	virtual std::string toString(const unsigned int indentLevel) const;
    };
}

#endif // __H5TYPE_HXX__
