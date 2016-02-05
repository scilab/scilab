/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
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
