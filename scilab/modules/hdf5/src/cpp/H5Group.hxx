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

#ifndef __H5GROUP_HXX__
#define __H5GROUP_HXX__

#include "H5Object.hxx"
#include "H5AttributesList.hxx"

namespace org_modules_hdf5
{
    
    class H5LinksList;
    class H5GroupsList;
    class H5DatasetsList;
    class H5TypesList;
    class H5File;

    class H5Group : public H5Object
    {
	hid_t group;
	const char * name;

    public:

	H5Group(H5Object & _parent, const char * name);
	H5Group(H5Object & _parent, hid_t _group, const char * _name);
	
	virtual ~H5Group();

	virtual hid_t getH5Id() const { return group; }
        virtual H5LinksList & getLinks();
        virtual H5GroupsList & getGroups();
        virtual H5DatasetsList & getDatasets();
        virtual H5TypesList & getTypes();
	virtual std::string getName() const { return std::string(name); }
	virtual std::string toString(const unsigned int indentLevel) const;
	virtual std::string dump(const unsigned int indentLevel = 0) const;
    };
}

#endif // __H5GROUP_HXX__
