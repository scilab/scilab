/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __SPLITSTRING_HXX__
#define __SPLITSTRING_HXX__

#include <string>
#include <vector>

namespace org_modules_xml
{
    /**
     * @file
     * @author Calixte DENIZET <calixte.denizet@scilab.org>
     *
     * Split a string into lines
     */
    class SplitString
    {

public:
        /**
         * Split string into lines
         * @param str the string to split
         * @param lines a vector which will contain lines
         */
        static void split(const std::string & str, std::vector < std::string > &lines)
        {
            std::string::size_type lastPos = str.find_first_not_of("\n", 0);
            std::string::size_type pos = str.find_first_of("\n", lastPos);

            while (std::string::npos != pos || std::string::npos != lastPos)
            {
                lines.push_back(str.substr(lastPos, pos - lastPos));
                lastPos = str.find_first_not_of("\n", pos);
                pos = str.find_first_of("\n", lastPos);
            }
        }
    };
}

#endif
