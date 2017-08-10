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

#include "H5ExternalLink.hxx"

namespace org_modules_hdf5
{

std::vector<std::string *> H5ExternalLink::getLinkTargets() const
{
    herr_t err;
    H5L_info_t info;
    char * buf = 0;
    std::vector<std::string *> ret;
    const char * filename = 0;
    const char * obj_path = 0;

    err = H5Lget_info(getParent().getH5Id(), getName().c_str(), &info, H5P_DEFAULT);
    if (err < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot get the link info"));
    }

    buf = new char[info.u.val_size];
    err = H5Lget_val(getParent().getH5Id(), getName().c_str(), buf, info.u.val_size, H5P_DEFAULT);
    if (err < 0)
    {
        delete[] buf;
        throw H5Exception(__LINE__, __FILE__, _("Cannot get the link target"));
    }

    // According to the doc the flags arg is useless
    // For the future: follow the evolution of this argument.
    err = H5Lunpack_elink_val(buf, info.u.val_size, 0, &filename, &obj_path);
    if (err < 0)
    {
        delete[] buf;
        throw H5Exception(__LINE__, __FILE__, _("Cannot get the link target"));
    }

    ret.reserve(2);
    ret[0] = new std::string(filename);
    ret[1] = new std::string(obj_path);

    delete[] buf;

    return ret;
}

std::string H5ExternalLink::getLinkType() const
{
    return "external";
}

void H5ExternalLink::getAccessibleAttribute(const std::string & _name, const int pos, void * pvApiCtx) const
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
        const char * _target[2];
        std::vector<std::string *> target = getLinkTargets();
        _target[0] = (*target[0]).c_str();
        _target[1] = (*target[1]).c_str();

        err = createMatrixOfString(pvApiCtx, pos, 1, 2, _target);
        target.erase(target.begin(), target.end());
        if (err.iErr)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot create a string on the stack."));
        }

        return;
    }

    H5Object::getAccessibleAttribute(_name, pos, pvApiCtx);
}

void H5ExternalLink::printLsInfo(std::ostringstream & os) const
{
    std::string str(getName());
    H5Object::getResizedString(str);
    std::vector<std::string *> target = getLinkTargets();

    os << str << "External Link {" << *target[0] << "//" << *target[1] << "}" << std::endl;

    target.erase(target.begin(), target.end());
}

std::string H5ExternalLink::ls() const
{
    std::ostringstream os;
    printLsInfo(os);

    return os.str();
}

std::string H5ExternalLink::dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel) const
{
    std::ostringstream os;
    std::vector<std::string *> target = getLinkTargets();

    os << H5Object::getIndentString(indentLevel) << "EXTERNAL_LINK \"" << name << "\" {" << std::endl
       << H5Object::getIndentString(indentLevel + 1) << "TARGETFILE \"" << *target[0] << "\"" << std::endl
       << H5Object::getIndentString(indentLevel + 1) << "TARGETPATH \"" << *target[1] << "\"" << std::endl
       << H5Object::getIndentString(indentLevel) << "}" << std::endl;

    target.erase(target.begin(), target.end());

    return os.str();
}

std::string H5ExternalLink::toString(const unsigned int indentLevel) const
{
    std::ostringstream os;
    std::vector<std::string *> target = getLinkTargets();
    std::string indentString = H5Object::getIndentString(indentLevel);

    os << indentString << _("Filename") << ": " << getFile().getFileName() << std::endl
       << indentString << _("Link type") << ": " << getLinkType() << std::endl
       << indentString << _("Link name") << ": " << name << std::endl
       << indentString << _("Link path") << ": " << getCompletePath() << std::endl
       << indentString << _("Link target file") << ": " << *target[0] << std::endl
       << indentString << _("Link target path") << ": " << *target[1];

    target.erase(target.begin(), target.end());

    return os.str();
}
}
