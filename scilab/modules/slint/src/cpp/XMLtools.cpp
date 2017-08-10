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

#include <cstdlib>

#include "XMLtools.hxx"
#include "SLint.hxx"
#include "SLintXMLException.hxx"
#include "UTF8.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "charEncoding.h"
#include "localization.h"
}

#define SLINT_XML_ERROR_BUFFER_SIZE 1024

namespace slint
{

std::ostringstream XMLtools::errorBuffer;

bool XMLtools::getDouble(xmlNode * node, const char * attrName, double & out)
{
    xmlAttr * attr = xmlHasProp(node, (const xmlChar *)attrName);
    if (attr)
    {
        out = std::atof((const char *)attr->children->content);
        return true;
    }
    return false;
}

bool XMLtools::getUIntVector(xmlNode * node, const char * attrName, std::vector<unsigned int> & out)
{
    xmlAttr * attr = xmlHasProp(node, (const xmlChar *)attrName);
    if (attr)
    {
        std::vector<std::string> toks = tokenize((const char *)attr->children->content, ',');
        for (const auto & tok : toks)
        {
            const int i = std::stoi(tok);
            if (i > 0)
            {
                out.emplace_back((unsigned int)i);
            }
        }
        return !toks.empty();
    }
    return false;
}

bool XMLtools::getInt(xmlNode * node, const char * attrName, int & out)
{
    xmlAttr * attr = xmlHasProp(node, (const xmlChar *)attrName);
    if (attr)
    {
        out = std::atoi((const char *)attr->children->content);
        return true;
    }
    return false;
}

bool XMLtools::getString(xmlNode * node, const char * attrName, std::string & out)
{
    xmlAttr * attr = xmlHasProp(node, (const xmlChar *)attrName);
    if (attr)
    {
        out = std::string((const char *)attr->children->content);
        return true;
    }
    return false;
}

bool XMLtools::getWString(xmlNode * node, const char * attrName, std::wstring & out)
{
    xmlAttr * attr = xmlHasProp(node, (const xmlChar *)attrName);
    if (attr)
    {
        wchar_t * content = to_wide_string((const char *)attr->children->content);
        out = std::wstring(content);
        FREE(content);

        return true;
    }
    return false;
}

bool XMLtools::getBool(xmlNode * node, const char * attrName, bool & out)
{
    xmlAttr * attr = xmlHasProp(node, (const xmlChar *)attrName);
    if (attr)
    {
        std::string val((const char *)attr->children->content);
        if (val == "true" || val == "1" || val == "yes")
        {
            out = true;
            return true;
        }
        else if (val == "false" || val == "0" || val == "no")
        {
            out = false;
            return true;
        }
    }
    return false;
}

xmlDoc * XMLtools::readXML(const std::wstring & path)
{
    const std::string fullpath = scilab::UTF8::toUTF8(SLint::getFullPath(path));
    xmlParserCtxt * ctxt = xmlNewParserCtxt();
    if (!ctxt)
    {
        throw SLintXMLException(fullpath, gettext("Cannot create a parser context"));
    }

    xmlSetGenericErrorFunc(ctxt, errorFunction);

    int options = XML_PARSE_NSCLEAN | XML_PARSE_NOBLANKS;
    xmlDoc * doc = xmlCtxtReadFile(ctxt, fullpath.c_str(), "UTF-8", options);
    if (!doc || !ctxt->valid)
    {
        std::string error = errorBuffer.str();
        errorBuffer.str("");
        errorBuffer.clear();
        throw SLintXMLException(fullpath, error);
    }

    xmlFreeParserCtxt(ctxt);

    return doc;
}

void XMLtools::errorFunction(void * ctx, const char * msg, ...)
{
    char str[SLINT_XML_ERROR_BUFFER_SIZE];
    va_list args;

    va_start(args, msg);
    vsnprintf(str, SLINT_XML_ERROR_BUFFER_SIZE, msg, args);
    va_end(args);

    errorBuffer << str;
}

std::vector<std::string> XMLtools::tokenize(const std::string & str, const char delim)
{
    std::vector<std::string> tokens;
    std::string token;
    std::for_each(str.begin(), str.end(), [&](char c)
    {
        if (c != ' ' && c != '\t')
        {
            if (c != delim)
            {
                token += c;
            }
            else if (!token.empty())
            {
                tokens.emplace_back(token);
                token.clear();
            }
        }
    });

    if (!token.empty())
    {
        tokens.emplace_back(token);
    }
    return tokens;
}

} // namespace slint
