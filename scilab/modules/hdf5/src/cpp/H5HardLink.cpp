/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include "H5Object.hxx"
#include "H5HardLink.hxx"

namespace org_modules_hdf5
{

H5Object & H5HardLink::getLinkedObject() const
{
    herr_t err;
    H5L_info_t info;
    hid_t obj;

    err = H5Lget_info(getParent().getH5Id(), name.c_str(), &info, H5P_DEFAULT);
    if (err < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot get the link info"));
    }

    obj = H5Oopen_by_addr(getFile().getH5Id(), info.u.address);
    if (obj < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot get linked object"));
    }

    try
    {
        return H5Object::getObject(*const_cast<H5HardLink *>(this), obj);
    }
    catch (const H5Exception & /*e*/)
    {
        H5Oclose(obj);
        throw;
    }
}

std::string H5HardLink::getLinkType() const
{
    return "hard";
}

void H5HardLink::getAccessibleAttribute(const std::string & _name, const int pos, void * pvApiCtx) const
{
    SciErr err;
    std::string lower(_name);
    std::transform(_name.begin(), _name.end(), lower.begin(), tolower);

    if (lower == "type")
    {
        const std::string linkType = getLinkType();
        const char * _type = linkType.c_str();
        err = createMatrixOfString(pvApiCtx, pos, 1, 1, &_type);
        if (err.iErr)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot create a string on the stack."));
        }

        return;
    }
    else if (lower == "target")
    {
        H5Object & obj = getLinkedObject();
        obj.createOnScilabStack(pos, pvApiCtx);

        return;
    }

    H5Object::getAccessibleAttribute(_name, pos, pvApiCtx);
}

std::string H5HardLink::dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel) const
{
    std::ostringstream os;
    H5Object & obj = getLinkedObject();

    os << H5Object::getIndentString(indentLevel) << "HARDLINK \"" << obj.getName() << "\"" << std::endl;

    delete &obj;

    return os.str();
}

std::string H5HardLink::toString(const unsigned int indentLevel) const
{
    std::ostringstream os;
    std::string indentString = H5Object::getIndentString(indentLevel);
    H5Object & obj = getLinkedObject();

    os << indentString << _("Filename") << ": " << getFile().getFileName() << std::endl
       << indentString << _("Link type") << ": " << getLinkType() << std::endl
       << indentString << _("Link name") << ": " << name << std::endl
       << indentString << _("Link path") << ": " << getCompletePath() << std::endl
       << indentString << _("Link target name") << ": " << obj.getName();

    delete &obj;

    return os.str();
}
}
