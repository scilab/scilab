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

#ifndef __H5ATTRIBUTESLIST_HXX__
#define __H5ATTRIBUTESLIST_HXX__

#include "H5ListObject.hxx"

namespace org_modules_hdf5
{
    class H5Attribute;

    class H5AttributesList : protected H5ListObject<H5Attribute>
    {

    public :
	
	H5AttributesList(H5Object & _parent);
	
	~H5AttributesList();

	void setObject(const unsigned int pos, H5Attribute & attribute);
	H5Attribute & getObject(const int pos);
	const unsigned int getSize() const;
	
	virtual std::string dump(const unsigned int indentLevel) const;
        virtual std::string toString(const unsigned int indentLevel) const;

    private:
	H5Attribute & getObject(const int pos, const bool checkPos);
    };
}

#endif // __H5ATTRIBUTESLIST_HXX__
