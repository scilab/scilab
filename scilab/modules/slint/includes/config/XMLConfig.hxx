/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __SLINT_XML_CONFIG_HXX__
#define __SLINT_XML_CONFIG_HXX__

#include "XMLtools.hxx"
#include "checkers/AllCheckers.hxx"
#include "SLintXMLException.hxx"

namespace types
{
class String;
}

namespace slint
{

class SLintOptions;

class XMLConfig
{

    typedef SLintChecker * (*CBType)(xmlNode *);

    static std::unordered_map<std::string, CBType> callbacks;

public:

    static void getOptions(const std::wstring & path, SLintOptions & options);

private:

    template<typename T>
    static SLintChecker * createFromXmlNode(xmlNode * node)
    {
        bool enable = true;
        XMLtools::getBool(node, "enable", enable);
        if (enable)
        {
            std::wstring id;
            XMLtools::getWString(node, "id", id);

            return new T(id);
        }

        return nullptr;
    }

    static std::unordered_map<std::string, CBType> initCallbacks();

};

} // namespace slint

#endif // __SLINT_XML_CONFIG_HXX__
