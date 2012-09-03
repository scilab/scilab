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

#include "H5Object.hxx"
#include "H5File.hxx"
#include "H5Group.hxx"
#include "H5Dataset.hxx"
#include "H5Type.hxx"
#include "H5AttributesList.hxx"

namespace org_modules_hdf5
{

    H5Object & H5Object::root = *new H5Object();

    H5Object::H5Object(H5Object & _parent) : parent(_parent), children(std::set<H5Object *>()), locked(false)
    {
	parent.registerChild(this);
    }

    H5Object::~H5Object()
    {
	locked = true;
	for (std::set<H5Object *>::iterator it = children.begin(); it != children.end(); it++)
	{
	    delete *it;
	}
	locked = false;
	parent.unregisterChild(this);
    }

    hid_t H5Object::getH5Id() const
    {
	return (hid_t)-1;
    }

    H5File & H5Object::getFile() const
    {
	const H5Object * sobj = this;
	const H5Object * obj = &parent;
	while (obj != &root)
	{
	    sobj = obj;
	    obj = &(obj->parent);
	}

	return *reinterpret_cast<H5File *>(const_cast<H5Object *>(sobj));
    }

    H5AttributesList & H5Object::getAttributes()
    {
	return *new H5AttributesList(*this);
    }

    H5Object & H5Object::getObject(H5Object & parent, hid_t obj)
    {
	H5O_info_t info;
	herr_t err = H5Oget_info(obj, &info);
	ssize_t size;
	char * name = 0;
	
	if (err <= 0)
	{
	    throw H5Exception(__LINE__, __FILE__, _("Cannot retrieve informations about the object"));
	}
	
	size = H5Iget_name(obj, 0, 0);
	name = (char *)MALLOC((size + 1) * sizeof(char));
	H5Iget_name(obj, name, size + 1);

	switch (info.type)
	{
	case H5O_TYPE_GROUP:
	    return *new H5Group(parent, obj, name);
	case H5O_TYPE_DATASET:
	    return *new H5Dataset(parent, obj, name);
	case H5O_TYPE_NAMED_DATATYPE:
	    return *new H5Type(parent, obj, name);
	case H5O_TYPE_UNKNOWN:
	default:
	    throw H5Exception(__LINE__, __FILE__, _("Unknown HDF5 object"));
	}
    }

    std::string H5Object::getCompletePath() const
    {
	std::string name = getName();
	if (this != &root && name != "")
	{
	    std::string path = parent.getCompletePath();
	    if (path == "/")
	    {
		return "/" + name;
	    }
	    else
	    {
		return path + name;
	    }
	}
	return "";
    }
}
