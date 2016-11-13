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

#ifndef __XMLVALIDATIONSCHEMA_HXX__
#define __XMLVALIDATIONSCHEMA_HXX__

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
 * Class to handle Schema validation.
 */
class XMLValidationSchema: public XMLValidation
{

public:
    /**
     * @param path the path of the validation file
     * @param error a string which will contain error messages
     */
    XMLValidationSchema(const char *path, std::string * error);

    ~XMLValidationSchema();

    bool validate(const XMLDocument & doc, std::string * error) const;
    bool validate(xmlTextReader * reader, std::string * error) const;

    const std::string toString() const;
};
}

#endif
