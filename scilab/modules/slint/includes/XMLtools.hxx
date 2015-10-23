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

#ifndef __SLINT_XML_TOOLS_HXX__
#define __SLINT_XML_TOOLS_HXX__

#include <sstream>
#include <string>
#include <vector>
#include <libxml/tree.h>

namespace slint
{

class XMLtools
{

public:

    static bool getDouble(xmlNode * node, const char * attrName, double & out);
    static bool getUIntVector(xmlNode * node, const char * attrName, std::vector<unsigned int> & out);
    static bool getInt(xmlNode * node, const char * attrName, int & out);
    static bool getString(xmlNode * node, const char * attrName, std::string & out);
    static bool getWString(xmlNode * node, const char * attrName, std::wstring & out);
    static bool getBool(xmlNode * node, const char * attrName, bool & out);
    static xmlDoc * readXML(const std::wstring & path);
    static void errorFunction(void * ctx, const char * msg, ...);
    static std::vector<std::string> tokenize(const std::string & str, const char delim);

private:

    static std::ostringstream errorBuffer;

};

} // namespace slint

#endif // __SLINT_XML_TOOLS_HXX__
