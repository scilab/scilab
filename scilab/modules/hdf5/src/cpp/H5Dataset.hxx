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

#ifndef __H5DATASET_HXX__
#define __H5DATASET_HXX__

#include "H5Object.hxx"
#include "H5Data.hxx"
#include "H5Dataspace.hxx"
#include "H5Type.hxx"
#include "H5AttributesList.hxx"

namespace org_modules_hdf5
{
    class H5Dataset : public H5Object
    {
	hid_t dataset;
	const char * name;

    public :
	
	H5Dataset(H5Object & _parent, const char * _name);
	H5Dataset(H5Object & _parent, hid_t _dataset, const char * _name);

	virtual ~H5Dataset();

	hid_t getH5Id() const { return dataset; }
	std::string getName() const { return std::string(name); }
	
	H5Data & getData();
	H5Dataspace & getSpace();
	H5Type & getDataType();
	
	virtual std::string dump(const unsigned int indentLevel) const;
	virtual std::string toString(const unsigned int indentLevel) const;
    };
}

#endif // __H5DATASET_HXX__
