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

#ifndef __H5DATASPACE_HXX__
#define __H5DATASPACE_HXX__

#include "H5Object.hxx"

namespace org_modules_hdf5
{
    
    class H5Dataspace : public H5Object
    {
	hid_t space;

    public:

	H5Dataspace(H5Object & _parent, hid_t _space);
	~H5Dataspace();

	virtual hid_t getH5Id();
    
	virtual std::string dump(unsigned int indentLevel) const;
	virtual std::string toString(unsigned int indentLevel) const;
    };
}

#endif // __H5DATASPACE_HXX__
