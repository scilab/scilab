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

#ifndef __XMLVALIDATIONRELAXNG_HXX__
#define __XMLVALIDATIONRELAXNG_HXX__

#include <string>

#include "xml.h"

namespace org_modules_xml
{

    class XMLValidation;
    class XMLDocument;

    /**
     * @file
     * @author Calixte DENIZET <calixte.denizet@scilab.org>
     *
     * Class to handle Relax NG validation.
     */
    class XMLValidationRelaxNG:public XMLValidation
    {

public:
        /**
         * @param path the path of the validation file
         * @param error a string which will contain error messages
         */
        XMLValidationRelaxNG(const char *path, std::string * error);

         ~XMLValidationRelaxNG();

        bool validate(const XMLDocument & doc, std::string * error) const;
        bool validate(xmlTextReader * reader, std::string * error) const;

        const std::string toString() const;
    };
}

#endif
