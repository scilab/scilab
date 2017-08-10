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

#include <string>

#include "config/cnes/CNESException.hxx"
#include "config/cnes/AnalysisConfiguration.hxx"
#include "XMLtools.hxx"

namespace slint
{

namespace CNES
{

AnalysisConfiguration AnalysisConfiguration::createFromXml(const std::wstring & path)
{
    xmlDoc * doc = slint::XMLtools::readXML(path);
    xmlNode * root = xmlDocGetRootElement(doc);

    std::string name((const char *)root->name);
    if (name == "analysisConfiguration")
    {
        AnalysisConfiguration analysisConfiguration(AnalysisConfigurationType::createFromXmlNode(root));
        xmlFreeDoc(doc);

        return analysisConfiguration;
    }
    else
    {
        xmlFreeDoc(doc);
        throw CNESException(L"Invalid analysis configuration file");
    }
}

} // namespace CNES

} // namespace slint
