/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "XMLObject.hxx"
#include "XMLValidation.hxx"
#include "XMLValidationDTD.hxx"
#include "XMLDocument.hxx"
#include "VariableScope.hxx"

extern "C"
{
#include "expandPathVariable.h"
#include "MALLOC.h"
#include "localization.h"
}

namespace org_modules_xml
{

XMLValidationDTD::XMLValidationDTD(const char *path, std::string * error): XMLValidation()
{
    internalValidate = false;
    char *expandedPath = expandPathVariable(const_cast<char *>(path));
    if (expandedPath)
    {
        validationFile = (void *)xmlParseDTD(0, (const xmlChar *)expandedPath);
        FREE(expandedPath);
        if (!validationFile)
        {
            if (errorBuffer)
            {
                delete errorBuffer;
            }
            errorBuffer = new std::string(gettext("Cannot parse the DTD"));

            *error = *errorBuffer;
        }
        else
        {
            openValidationFiles.push_back(this);
        }
    }
    else
    {
        *error = std::string(gettext("Invalid file name: ")) + std::string(path);
    }

    scope->registerPointers(validationFile, this);
    id = scope->getVariableId(*this);
}

XMLValidationDTD::XMLValidationDTD(): XMLValidation()
{
    validationFile = 0;
    internalValidate = true;
    id = scope->getVariableId(*this);
    openValidationFiles.push_back(this);
}

XMLValidationDTD::~XMLValidationDTD()
{
    scope->unregisterPointer(validationFile);
    scope->removeId(id);

    if (validationFile)
    {
        xmlFreeDtd(getValidationFile < xmlDtd > ());
    }

    if (validationFile || internalValidate)
    {
        openValidationFiles.remove(this);
        if (openValidationFiles.size() == 0 && XMLDocument::getOpenDocuments().size() == 0)
        {
            resetScope();
        }
    }

    if (errorBuffer)
    {
        delete errorBuffer;

        errorBuffer = 0;
    }
}

bool XMLValidationDTD::validate(const XMLDocument & doc, std::string * error) const
{
    bool ret;
    xmlValidCtxt *vctxt = xmlNewValidCtxt();

    if (errorBuffer)
    {
        delete errorBuffer;
    }
    errorBuffer = new std::string("");

    if (!vctxt)
    {
        errorBuffer->append(gettext("Cannot create a valid context"));
        *error = *errorBuffer;
        return false;
    }

    vctxt->error = (xmlValidityErrorFunc) XMLValidation::errorFunction;

    ret = xmlValidateDtd(vctxt, doc.getRealDocument(), getValidationFile < xmlDtd > ()) == 1;

    vctxt->error = 0;
    xmlFreeValidCtxt(vctxt);

    if (!ret)
    {
        *error = *errorBuffer;
    }

    return ret;
}

bool XMLValidationDTD::validate(xmlTextReader * reader, std::string * error) const
{
    int last;
    int valid;

    if (errorBuffer)
    {
        delete errorBuffer;
    }
    errorBuffer = new std::string("");

    if (!internalValidate)
    {
        errorBuffer->append(gettext("Due to a libxml2 limitation, it is not possible to validate a document against an external DTD\nPlease see help xmlValidate.\n"));
        *error = *errorBuffer;
        return false;
    }

    xmlTextReaderSetParserProp(reader, XML_PARSER_VALIDATE, 1);
    xmlTextReaderSetErrorHandler(reader, (xmlTextReaderErrorFunc) XMLValidation::errorReaderFunction, 0);
    while ((last = xmlTextReaderRead(reader)) == 1) ;
    valid = xmlTextReaderIsValid(reader);

    xmlTextReaderSetErrorHandler(reader, 0, 0);
    xmlFreeTextReader(reader);

    if (last == -1 || valid != 1)
    {
        *error = *errorBuffer;
        return false;
    }

    return true;
}

const std::string XMLValidationDTD::toString() const
{
    std::ostringstream oss;
    xmlDtd *dtd = getValidationFile < xmlDtd > ();

    oss << "XML DTD" << std::endl;
    oss << "name: " << (dtd->name ? (const char *)dtd->name : "") << std::endl;
    oss << "external ID: " << (dtd->ExternalID ? (const char *)dtd->ExternalID : "") << std::endl;
    oss << "system ID: " << (dtd->SystemID ? (const char *)dtd->SystemID : "");

    return oss.str();
}
}
