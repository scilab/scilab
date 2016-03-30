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

#include "XMLObject.hxx"
#include "XMLValidation.hxx"

extern "C"
{
#include "expandPathVariable.h"
#include "sci_malloc.h"
#include "localization.h"
}

#define BUFFER_SIZE 1024

namespace org_modules_xml
{

std::string XMLValidation::errorBuffer;
std::list<XMLValidation *> XMLValidation::openValidationFiles;

XMLValidation::XMLValidation(): XMLObject(), validationFile(0)
{
    scilabType = XMLVALID;
}

void XMLValidation::errorFunction(void *ctx, const char *msg, ...)
{
    char str[BUFFER_SIZE];
    va_list args;

    va_start(args, msg);
#ifdef _MSC_VER
    _vsnprintf(str, BUFFER_SIZE, msg, args);
#else
    vsnprintf(str, BUFFER_SIZE, msg, args);
#endif
    va_end(args);
    errorBuffer.append(str);
}

void XMLValidation::errorReaderFunction(void * arg, const char * msg, xmlParserSeverities severity, xmlTextReaderLocatorPtr locator)
{
    std::ostringstream oss;

    oss << xmlTextReaderLocatorBaseURI(locator) << gettext(" at line ")
        << xmlTextReaderLocatorLineNumber(locator) << std::endl
        << msg << std::endl;

    errorBuffer.append(oss.str());
}


bool XMLValidation::validate(const std::string & xmlCode, std::string * error) const
{
    xmlParserInputBuffer * buffer = xmlParserInputBufferCreateMem(xmlCode.c_str(), (int)xmlCode.size(), (xmlCharEncoding) 0);
    if (!buffer)
    {
        error->append(gettext("Cannot create a buffer"));
        return false;
    }

    xmlTextReader * reader = xmlNewTextReader(buffer, 0);
    if (!reader)
    {
        xmlFreeParserInputBuffer(buffer);
        error->append(gettext("Cannot create a reader"));
        return false;
    }

    bool valid = validate(reader, error);
    xmlFreeParserInputBuffer(buffer);

    return valid;
}

bool XMLValidation::validate(const char *path, std::string * error)const
{
    char *expandedPath = expandPathVariable(const_cast<char *>(path));
    if (expandedPath)
    {
        xmlTextReader *reader = xmlNewTextReaderFilename(expandedPath);
        FREE(expandedPath);
        if (!reader)
        {
            error->append(gettext("Invalid file"));
            return false;
        }

        return validate(reader, error);
    }
    else
    {
        *error = std::string(gettext("Invalid file name: ")) + std::string(path);
        return false;
    }
}

const std::list<XMLValidation *>& XMLValidation::getOpenValidationFiles()
{
    return openValidationFiles;
}

void XMLValidation::closeAllValidationFiles()
{
    int size = (int)openValidationFiles.size();
    XMLValidation **arr = new XMLValidation *[size];
    int j = 0;

    for (std::list < XMLValidation * >::iterator i = openValidationFiles.begin(); i != openValidationFiles.end(); i++, j++)
    {
        arr[j] = *i;
    }
    for (j = 0; j < size; j++)
    {
        delete arr[j];
    }
    delete[]arr;
}
}

