/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "dynamic_modules.hxx"
#include "context.hxx"
#include "funcmanager.hxx"
#include "configvariable.hxx"

extern "C"
{
#include "loadOnUseClassPath.h"
    //XML API
#include <libxml/xpath.h>
#include <libxml/xmlreader.h>
#include "MALLOC.h"
}

using namespace types;
using namespace std;

vectGateway loadGatewaysName(wstring _wstModuleName)
{
    vectGateway vect;
    wstring wstPath = ConfigVariable::getSCIPath();
    wstring wstModuleName = wstPath + L"/modules/" + _wstModuleName + L"/sci_gateway/" + _wstModuleName + L"_gateway.xml";

    char* pstModuleName = wide_string_to_UTF8(wstModuleName.c_str());

    /* Don't care about line return / empty line */
    xmlKeepBlanksDefault(0);
    // parse file
    xmlDocPtr doc;
    xmlXPathContextPtr xpathCtxt = NULL;
    xmlXPathObjectPtr xpathObj = NULL;

    doc = xmlParseFile(pstModuleName);
    if (doc == NULL)
    {
        std::cout << "Error: Could not parse file " << pstModuleName << std::endl;
        FREE(pstModuleName);
        return vect;
    }
    FREE(pstModuleName);

    xpathCtxt = xmlXPathNewContext(doc);
    xpathObj = xmlXPathEval((const xmlChar*)"//module/gateway", xpathCtxt);
    if (xpathObj && xpathObj->nodesetval->nodeMax)
    {
        /* the Xpath has been understood and there are node */
        for (int	i = 0 ; i < xpathObj->nodesetval->nodeNr ; i++)
        {
            GatewayStr str;

            xmlAttrPtr attrib = xpathObj->nodesetval->nodeTab[i]->properties;
            /* Get the properties of <module>  */
            while (attrib != NULL)
            {
                /* loop until when have read all the attributes */
                if (xmlStrEqual(attrib->name, (const xmlChar*)"name"))
                {
                    wchar_t * ws = to_wide_string((const char*)attrib->children->content);
                    str.wstName = ws;
                    FREE(ws);
                }
                else if (xmlStrEqual(attrib->name, (const xmlChar*)"function"))
                {
                    wchar_t * ws = to_wide_string((const char*)attrib->children->content);
                    str.wstFunction = ws;
                    FREE(ws);
                }
                else if (xmlStrEqual(attrib->name, (const xmlChar*)"type"))
                {
                    str.iType = (types::Function::FunctionType)atoi((const char*)attrib->children->content);
                }
                attrib = attrib->next;
            }
            vect.push_back(str);
        }
    }

    if (xpathObj)
    {
        xmlXPathFreeObject(xpathObj);
    }

    if (xpathCtxt)
    {
        xmlXPathFreeContext(xpathCtxt);
    }

    xmlFreeDoc(doc);

    return vect;
}

//Scinotes module

bool ScinotesModule::loadedDep = false;
void ScinotesModule::LoadDeps(void)
{
    if (loadedDep == false)
    {
        loadOnUseClassPath("SciNotes");
        loadedDep = true;
    }
}

int ScinotesModule::Load()
{
    wstring wstModuleName = L"scinotes";
#ifdef _MSC_VER
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_2);
#else
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(wstModuleName);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].wstFunction, vect[i].wstName, pwstLibName, vect[i].iType, &ScinotesModule::LoadDeps, wstModuleName));
    }

    FREE(pwstLibName);

    return 1;
}

//Functions module
int FunctionsModule::Load()
{
    wstring wstPath = L"functions";
#ifdef _MSC_VER
    wstring wstModuleName = L"functions_gw";
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    wstring wstModuleName = L"functions";
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(wstPath);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].wstFunction, vect[i].wstName, pwstLibName, vect[i].iType, NULL, wstModuleName));
    }

    FREE(pwstLibName);

    return 1;
}

int StatisticsModule::Load()
{
    wstring wstPath = L"statistics";
#ifdef _MSC_VER
    wstring wstModuleName = L"statistics_gw";
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    wstring wstModuleName = L"statistics";
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(wstPath);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].wstFunction, vect[i].wstName, pwstLibName, vect[i].iType, NULL, wstModuleName));
    }

    FREE(pwstLibName);

    return 1;
}

int SignalProcessingModule::Load()
{
    wstring wstPath = L"signal_processing";
#ifdef _MSC_VER
    wstring wstModuleName = L"signal_processing_gw";
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    wstring wstModuleName = L"signal_processing";
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(wstPath);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].wstFunction, vect[i].wstName, pwstLibName, vect[i].iType, NULL, wstModuleName));
    }

    FREE(pwstLibName);

    return 1;
}

void HelptoolsModule::LoadDeps(void)
{
    if (loadedDep == false)
    {
        loadOnUseClassPath("documentationGeneration");
        loadedDep = true;
    }
}

bool HelptoolsModule::loadedDep = false;
int HelptoolsModule::Load()
{
    wstring wstModuleName = L"helptools";
#ifdef _MSC_VER
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif

    vectGateway vect = loadGatewaysName(wstModuleName);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].wstFunction, vect[i].wstName, pwstLibName, vect[i].iType, &HelptoolsModule::LoadDeps, wstModuleName));
    }

    FREE(pwstLibName);

    return 1;
}

int MatioModule::Load()
{
    wstring wstPath = L"matio";
#ifdef _MSC_VER
    wstring wstModuleName = L"matio_gw";
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    wstring wstModuleName = L"matio";
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(wstPath);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].wstFunction, vect[i].wstName, pwstLibName, vect[i].iType, NULL, wstModuleName));
    }

    FREE(pwstLibName);

    return 1;
}

int Hdf5Module::Load()
{
    wstring wstModuleName = L"hdf5";
#ifdef _MSC_VER
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(wstModuleName);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].wstFunction, vect[i].wstName, pwstLibName, vect[i].iType, NULL, wstModuleName));
    }

    FREE(pwstLibName);

    return 1;
}

int ActionBindingModule::Load()
{
    wstring wstPath = L"action_binding";
#ifdef _MSC_VER
    wstring wstModuleName = L"action_binding_gw";
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_2);
#else
    wstring wstModuleName = L"action_binding";
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(wstPath);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].wstFunction, vect[i].wstName, pwstLibName, vect[i].iType, NULL, wstModuleName));
    }

    FREE(pwstLibName);

    return 1;
}

int DifferentialEquationsModule::Load()
{
    wstring wstPath = L"differential_equations";
#ifdef _MSC_VER
    wstring wstModuleName = L"differential_equations_gw";
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    wstring wstModuleName = L"differential_equations";
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(wstPath);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].wstFunction, vect[i].wstName, pwstLibName, vect[i].iType, NULL, wstModuleName));
    }

    FREE(pwstLibName);

    return 1;
}

int SpreadsheetModule::Load()
{
    wstring wstModuleName = L"spreadsheet";
#ifdef _MSC_VER
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(wstModuleName);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].wstFunction, vect[i].wstName, pwstLibName, vect[i].iType, NULL, wstModuleName));
    }

    FREE(pwstLibName);

    return 1;
}

int InterpolationModule::Load()
{
    wstring wstPath = L"interpolation";
#ifdef _MSC_VER
    wstring wstModuleName = L"interpolation";
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    wstring wstModuleName = L"interpolation";
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(wstPath);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].wstFunction, vect[i].wstName, pwstLibName, vect[i].iType, NULL, wstModuleName));
    }

    FREE(pwstLibName);

    return 1;
}

int SoundModule::Load()
{
    wstring wstPath = L"sound";
#ifdef _MSC_VER
    wstring wstModuleName = L"sound";
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    wstring wstModuleName = L"sound";
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(wstPath);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].wstFunction, vect[i].wstName, pwstLibName, vect[i].iType, NULL, wstModuleName));
    }

    FREE(pwstLibName);

    return 1;
}

int RandlibModule::Load()
{
    wstring wstModuleName = L"randlib";
#ifdef _MSC_VER
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(wstModuleName);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].wstFunction, vect[i].wstName, pwstLibName, vect[i].iType, NULL, wstModuleName));
    }

    FREE(pwstLibName);

    return 1;
}

int CacsdModule::Load()
{
    wstring wstModuleName = L"cacsd";
#ifdef _MSC_VER
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(wstModuleName);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].wstFunction, vect[i].wstName, pwstLibName, vect[i].iType, NULL, wstModuleName));
    }

    FREE(pwstLibName);

    return 1;
}

int UmfpackModule::Load()
{
    wstring wstModuleName = L"umfpack";
#ifdef _MSC_VER
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(wstModuleName);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].wstFunction, vect[i].wstName, pwstLibName, vect[i].iType, NULL, wstModuleName));
    }

    FREE(pwstLibName);

    return 1;
}

int OptimizationModule::Load()
{
    wstring wstModuleName = L"optimization";
#ifdef _MSC_VER
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(wstModuleName);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].wstFunction, vect[i].wstName, pwstLibName, vect[i].iType, NULL, wstModuleName));
    }

    FREE(pwstLibName);

    return 1;
}

int SpecialFunctionModule::Load()
{
    wstring wstModuleName = L"special_functions";
#ifdef _MSC_VER
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(wstModuleName);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].wstFunction, vect[i].wstName, pwstLibName, vect[i].iType, NULL, wstModuleName));
    }

    FREE(pwstLibName);

    return 1;
}

int GraphicExportModule::Load()
{
    wstring wstModuleName = L"graphic_export";
#ifdef _MSC_VER
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_2);
#else
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(wstModuleName);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].wstFunction, vect[i].wstName, pwstLibName, vect[i].iType, NULL, wstModuleName));
    }

    FREE(pwstLibName);

    return 1;
}

int ArnoldiModule::Load()
{
    wstring wstModuleName = L"arnoldi";
#ifdef _MSC_VER
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(wstModuleName);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].wstFunction, vect[i].wstName, pwstLibName, vect[i].iType, NULL, wstModuleName));
    }

    FREE(pwstLibName);

    return 1;
}

int CallScilabModule::Load()
{
    wstring wstModuleName = L"call_scilab";
#ifdef _MSC_VER
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(wstModuleName);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].wstFunction, vect[i].wstName, pwstLibName, vect[i].iType, NULL, wstModuleName));
    }

    FREE(pwstLibName);

    return 1;
}

int CompletionModule::Load()
{
    wstring wstModuleName = L"completion";
#ifdef _MSC_VER
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_2);
#else
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(wstModuleName);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].wstFunction, vect[i].wstName, pwstLibName, vect[i].iType, NULL, wstModuleName));
    }

    FREE(pwstLibName);

    return 1;
}

int XmlModule::Load()
{
    wstring wstModuleName = L"xml";
#ifdef _MSC_VER
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(wstModuleName);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].wstFunction, vect[i].wstName, pwstLibName, vect[i].iType, NULL, wstModuleName));
    }

    return 1;
}
