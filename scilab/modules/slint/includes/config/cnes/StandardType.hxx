/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __SLINT_CNES_STANDARD_TYPE_HXX__
#define __SLINT_CNES_STANDARD_TYPE_HXX__

#include <string>
#include <vector>
#include <libxml/tree.h>

#include "StandardRuleType.hxx"

namespace slint
{

namespace CNES
{

class StandardType
{

    const std::string reference;
    const std::string type;
    const std::string version;
    const std::string date;

    std::vector<StandardRuleType> rule;

public:

    StandardType(const std::string & _reference, const std::string & _type, const std::string & _version, const std::string & _date) : reference(_reference), type(_type), version(_version), date(_date) { }

    inline const std::string & getReference() const
    {
        return reference;
    }
    inline const std::string & getType() const
    {
        return type;
    }
    inline const std::string & getVersion() const
    {
        return version;
    }
    inline const std::string & getDate() const
    {
        return date;
    }
    inline const std::vector<StandardRuleType> & getRule() const
    {
        return rule;
    }

    inline void add(const StandardRuleType & rt)
    {
        rule.emplace_back(rt);
    }

    static StandardType createFromXmlNode(xmlNode * node);
};


} // namespace CNES

} // namespace slint

#endif // __SLINT_CNES_STANDARD_TYPE_HXX__
