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

#include "config/cnes/ExcludedProjectFileType.hxx"
#include "XMLtools.hxx"

namespace slint
{

namespace CNES
{

ExcludedProjectFileType ExcludedProjectFileType::createFromXmlNode(xmlNode * node)
{
    std::string filename;
    slint::XMLtools::getString(node, "excludedProjectFileName", filename);

    return ExcludedProjectFileType(filename);
}

} // namespace CNES

} // namespace slint
