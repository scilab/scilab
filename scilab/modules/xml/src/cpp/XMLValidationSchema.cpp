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
#include "XMLValidationSchema.hxx"
#include "XMLDocument.hxx"
#include "VariableScope.hxx"

extern "C"
{
#include "expandPathVariable.h"
#include "MALLOC.h"
#include "localization.h"
#include "BOOL.h"
}

namespace org_modules_xml
{

XMLValidationSchema::XMLValidationSchema(const char *path, std::string * error): XMLValidation()
{
    char *expandedPath = expandPathVariable(const_cast < char *>(path));
    if (expandedPath)
    {
        xmlSchemaParserCtxt *pctxt = xmlSchemaNewParserCtxt(expandedPath);
        FREE(expandedPath);
        if (!pctxt)
        {
            if (errorBuffer)
            {
                delete errorBuffer;
            }
            errorBuffer = new std::string(gettext("Cannot create a validation context"));
            *error = *errorBuffer;
        }
        else
        {
            validationFile = (void *)xmlSchemaParse(pctxt);
            xmlSchemaFreeParserCtxt(pctxt);
            if (!validationFile)
            {
                if (errorBuffer)
                {
                    delete errorBuffer;
                }
                errorBuffer = new std::string(gettext("Cannot parse the schema"));
                *error = *errorBuffer;
            }
            else
            {
                openValidationFiles.push_back(this);
            }
        }
    }
    else
    {
        *error = std::string(gettext("Invalid file name: ")) + std::string(path);
    }
    scope->registerPointers(validationFile, this);
    id = scope->getVariableId(*this);
}

XMLValidationSchema::~XMLValidationSchema()
{
    scope->unregisterPointer(validationFile);
    scope->removeId(id);
    if (validationFile)
    {
        xmlSchemaFree((xmlSchema *) validationFile);
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

bool XMLValidationSchema::validate(const XMLDocument & doc, std::string * error) const
{
    bool ret;
    xmlSchemaValidCtxt *vctxt = xmlSchemaNewValidCtxt((xmlSchema *) validationFile);

    if (errorBuffer)
    {
        delete errorBuffer;
    }
    errorBuffer = new std::string("");

    if (!vctxt)
    {
        errorBuffer->append(gettext("Cannot create a validation context"));
        *error = *errorBuffer;
        return false;
    }

    xmlSchemaSetValidErrors(vctxt, (xmlSchemaValidityErrorFunc) XMLValidation::errorFunction, 0, 0);

    ret = BOOLtobool(xmlSchemaValidateDoc(vctxt, doc.getRealDocument()));

    xmlSchemaSetValidErrors(vctxt, 0, 0, 0);
    xmlSchemaFreeValidCtxt(vctxt);

    if (ret)
    {
        *error = *errorBuffer;
    }

    return ret == 0;
}

bool XMLValidationSchema::validate(xmlTextReader * reader, std::string * error) const
{
    xmlSchemaValidCtxt *vctxt = 0;
    int last;
    int valid;

    if (errorBuffer)
    {
        delete errorBuffer;
    }
    errorBuffer = new std::string();

    if (!reader)
    {
        errorBuffer->append(gettext("Cannot read the stream"));
        *error = *errorBuffer;
        return false;
    }

    vctxt = xmlSchemaNewValidCtxt(getValidationFile < xmlSchema > ());
    if (!vctxt)
    {
        errorBuffer->append(gettext("Cannot create a validation context"));
        *error = *errorBuffer;
        return false;
    }

    xmlSchemaSetValidErrors(vctxt, (xmlSchemaValidityErrorFunc) XMLValidation::errorFunction, 0, 0);
    xmlTextReaderSetErrorHandler(reader, (xmlTextReaderErrorFunc) XMLValidation::errorReaderFunction, 0);
    xmlTextReaderSchemaValidateCtxt(reader, vctxt, 0);

    while ((last = xmlTextReaderRead(reader)) == 1) ;
    valid = xmlTextReaderIsValid(reader);

    xmlTextReaderSetErrorHandler(reader, 0, 0);
    xmlSchemaSetValidErrors(vctxt, 0, 0, 0);
    xmlFreeTextReader(reader);
    xmlSchemaFreeValidCtxt(vctxt);

    if (last == -1 || valid != 1)
    {
        *error = *errorBuffer;
        return false;
    }

    return true;
}

const std::string XMLValidationSchema::toString() const
{
    std::ostringstream oss;
    xmlSchema *schema = getValidationFile < xmlSchema > ();

    oss << "XML Schema" << std::endl;
    oss << "name: " << (schema->name ? (const char *)schema->name : "") << std::endl;
    oss << "target namespace: " << (schema->targetNamespace ? (const char *)schema->targetNamespace : "") << std::endl;
    oss << "version: " << (schema->version ? (const char *)schema->version : "");

    return oss.str();
}

}
