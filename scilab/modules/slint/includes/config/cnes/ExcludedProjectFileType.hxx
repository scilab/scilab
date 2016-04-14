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

#ifndef __SLINT_CNES_EXCLUDED_PROJECT_FILE_TYPE_HXX__
#define __SLINT_CNES_EXCLUDED_PROJECT_FILE_TYPE_HXX__

#include <string>
#include <libxml/tree.h>

namespace slint
{

namespace CNES
{

class ExcludedProjectFileType
{

    const std::string filename;

public:

    ExcludedProjectFileType(const std::string & _filename) : filename(_filename) { }

    inline const std::string & getFilename() const
    {
        return filename;
    }

    static ExcludedProjectFileType createFromXmlNode(xmlNode * node);

};


} // namespace CNES

} // namespace slint

#endif // __SLINT_CNES_EXCLUDED_PROJECT_FILE_TYPE_HXX__
