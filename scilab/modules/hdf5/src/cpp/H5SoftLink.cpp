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

#include "H5SoftLink.hxx"

namespace org_modules_hdf5
{

std::string H5SoftLink::getLinkValue() const
{
    herr_t err;
    H5L_info_t info;
    char * buf = 0;
    std::string ret;

    err = H5Lget_info(getParent().getH5Id(), name.c_str(), &info, H5P_DEFAULT);
    if (err < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot get the link info"));
    }

    buf = new char[info.u.val_size];
    err = H5Lget_val(getParent().getH5Id(), name.c_str(), static_cast<void *>(buf), info.u.val_size, H5P_DEFAULT);
    if (err < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot get the link target"));
    }

    ret = std::string(buf);
    delete[] buf;

    return ret;
}

std::string H5SoftLink::getLinkType() const
{
    return "soft";
}

void H5SoftLink::getAccessibleAttribute(const std::string & _name, const int pos, void * pvApiCtx) const
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
        const std::string linkValue = getLinkValue();
        const char * _target = linkValue.c_str();
        err = createMatrixOfString(pvApiCtx, pos, 1, 1, &_target);
        if (err.iErr)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot create a string on the stack."));
        }

        return;
    }

    H5Object::getAccessibleAttribute(_name, pos, pvApiCtx);
}

std::string H5SoftLink::dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel) const
{
    std::ostringstream os;

    os << H5Object::getIndentString(indentLevel) << "SOFTLINK \"" << name << "\" {" << std::endl
       << H5Object::getIndentString(indentLevel + 1) << "LINKTARGET \"" << getLinkValue() << "\"" << std::endl
       << H5Object::getIndentString(indentLevel) << "}" << std::endl;

    return os.str();
}

void H5SoftLink::printLsInfo(std::ostringstream & os) const
{
    std::string str(getName());
    H5Object::getResizedString(str);

    os << str << "Soft Link {" << getLinkValue() << "}" << std::endl;
}

std::string H5SoftLink::ls() const
{
    std::ostringstream os;
    printLsInfo(os);

    return os.str();
}

std::string H5SoftLink::toString(const unsigned int indentLevel) const
{
    std::ostringstream os;
    std::string indentString = H5Object::getIndentString(indentLevel);

    os << indentString << _("Filename") << ": " << getFile().getFileName() << std::endl
       << indentString << _("Link type") << ": " << getLinkType() << std::endl
       << indentString << _("Link name") << ": " << name << std::endl
       << indentString << _("Link path") << ": " << getCompletePath() << std::endl
       << indentString << _("Link target") << ": " << getLinkValue() << std::endl;

    return os.str();
}
}
