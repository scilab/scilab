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
#include "H5LinksList.hxx"
#include "H5GroupsList.hxx"
#include "H5DatasetsList.hxx"
#include "H5TypesList.hxx"

namespace org_modules_hdf5
{

    H5Group::H5Group(H5Object & _parent, const char * _name) : H5Object(_parent), name(_name)
    {
	group = H5Gopen(_parent.getH5Id(), name, H5P_DEFAULT);
	if (group < 0)
	{
	    throw H5Exception(__LINE__, __FILE__, _("Cannot open the group %s."), name);
	}
    }

    H5Group::H5Group(H5Object & _parent, hid_t _group, const char * _name) : H5Object(_parent), group(_group), name(_name)
    {

    }

    H5Group::~H5Group()
    {
	if (group >= 0)
	{
	    H5Gclose(group);
	}
	if (name)
	{
	    FREE(const_cast<char *>(name));
	}
    }

    H5LinksList & H5Group::getLinks()
    {
	return *new H5LinksList(*this);
    }

    H5GroupsList & H5Group::getGroups()
    {
	return *new H5GroupsList(*this);
    }

    H5DatasetsList & H5Group::getDatasets()
    {
	return *new H5DatasetsList(*this);
    }

    H5TypesList & H5Group::getTypes()
    {
	return *new H5TypesList(*this);
    }

    std::string H5Group::dump(const unsigned int indentLevel) const
    {
	std::ostringstream os;
	H5AttributesList & attrs = const_cast<H5Group *>(this)->getAttributes();
	H5LinksList & links = const_cast<H5Group *>(this)->getLinks();
	
	os << H5Object::getIndentString(indentLevel) << "GROUP \"" << name << "\" {" << std::endl
	   << attrs.dump(indentLevel + 1)
	   << links.dump(indentLevel + 1)
	   << H5Object::getIndentString(indentLevel) << "}" << std::endl;

	delete &attrs;
	delete &links;

	return os.str();
    }

    std::string H5Group::toString(const unsigned int indentLevel) const
    {
	std::ostringstream os;
	std::string indentString = H5Object::getIndentString(indentLevel);
	const H5GroupsList & groups = const_cast<H5Group *>(this)->getGroups();
	const H5DatasetsList & datasets = const_cast<H5Group *>(this)->getDatasets();
	const H5TypesList & types = const_cast<H5Group *>(this)->getTypes();
	const H5AttributesList & attrs = const_cast<H5Group *>(this)->getAttributes();
	
	os << indentString << _("Filename") << ": " << getFile().getFileName() << std::endl
	   << indentString << _("Group name") << ": " << name << std::endl
	   << indentString << _("Group path") << ": " << getCompletePath() << std::endl
	   << indentString << _("Number of attributes") << ": " << attrs.getSize() << std::endl
	   << indentString << _("Number of named groups") << ": " << groups.getSize() << std::endl
	   << indentString << _("Number of named datasets") << ": " << datasets.getSize() << std::endl
	   << indentString << _("Number of named types") << ": " << types.getSize();

	delete &groups;
	delete &datasets;
	delete &types;
	delete &attrs;

	return os.str();
    }
}
