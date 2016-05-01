/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Antoine ELIAS
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

#include "dynamic_modules.hxx"
#include "context.hxx"
#include "configvariable.hxx"

extern "C"
{
#include "loadOnUseClassPath.h"
    //XML API
#include <libxml/xpath.h>
#include <libxml/xmlreader.h>
#include "sci_malloc.h"
#include "configvariable_interface.h"
}

vectGateway loadGatewaysName(const std::wstring& _wstModuleName)
{
    vectGateway vect;
    std::wstring wstPath = ConfigVariable::getSCIPath();
    std::wstring wstModuleName = wstPath + L"/modules/" + _wstModuleName + L"/sci_gateway/" + _wstModuleName + L"_gateway.xml";

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
int ScinotesModule::LoadDeps(const std::wstring& _functionName)
{
    if (loadedDep == false)
    {
        loadOnUseClassPath("SciNotes");
        loadedDep = true;
    }

    return 1;
}

int ScinotesModule::Load()
{
    std::wstring wstModuleName = L"scinotes";
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
    std::wstring wstPath = L"functions";
#ifdef _MSC_VER
    std::wstring wstModuleName = L"functions_gw";
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    std::wstring wstModuleName = L"functions";
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
    std::wstring wstPath = L"statistics";
#ifdef _MSC_VER
    std::wstring wstModuleName = L"statistics_gw";
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    std::wstring wstModuleName = L"statistics";
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
    std::wstring wstPath = L"signal_processing";
#ifdef _MSC_VER
    std::wstring wstModuleName = L"signal_processing_gw";
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    std::wstring wstModuleName = L"signal_processing";
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

int HelptoolsModule::LoadDeps(const std::wstring& _functionName)
{
    if (loadedDep == false)
    {
        loadOnUseClassPath("documentationGeneration");
        loadedDep = true;
    }

    return 1;
}

bool HelptoolsModule::loadedDep = false;
int HelptoolsModule::Load()
{
    std::wstring wstModuleName = L"helptools";
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
    std::wstring wstPath = L"matio";
#ifdef _MSC_VER
    std::wstring wstModuleName = L"matio_gw";
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    std::wstring wstModuleName = L"matio";
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
    std::wstring wstModuleName = L"hdf5";
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
    std::wstring wstPath = L"action_binding";
#ifdef _MSC_VER
    std::wstring wstModuleName = L"action_binding_gw";
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_2);
#else
    std::wstring wstModuleName = L"action_binding";
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
    std::wstring wstModuleName = L"spreadsheet";
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
    std::wstring wstPath = L"interpolation";
#ifdef _MSC_VER
    std::wstring wstModuleName = L"interpolation";
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    std::wstring wstModuleName = L"interpolation";
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
    std::wstring wstPath = L"sound";
#ifdef _MSC_VER
    std::wstring wstModuleName = L"sound";
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    std::wstring wstModuleName = L"sound";
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
    std::wstring wstModuleName = L"randlib";
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
    std::wstring wstModuleName = L"umfpack";
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
    std::wstring wstModuleName = L"optimization";
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

int SpecialFunctionsModule::Load()
{
    std::wstring wstModuleName = L"special_functions";
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
    std::wstring wstModuleName = L"graphic_export";
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
    std::wstring wstModuleName = L"arnoldi";
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
    std::wstring wstModuleName = L"call_scilab";
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
    std::wstring wstModuleName = L"completion";
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
    std::wstring wstModuleName = L"xml";
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

int ScicosModule::Load()
{
    std::wstring wstModuleName = L"scicos";
    const wchar_t* wstLibName = wstModuleName.c_str();
    if (getScilabMode() == SCILAB_NWNI)
    {
        wstLibName = L"scicos-cli";
    }
#ifdef _MSC_VER
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstLibName, DYNLIB_NAME_FORMAT_1);
#else
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstLibName, DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(wstModuleName);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].wstFunction, vect[i].wstName, pwstLibName, vect[i].iType, NULL, wstModuleName));
    }

    FREE(pwstLibName);
    return 1;
}

bool XcosModule::loadedDep = false;
int XcosModule::LoadDeps(const std::wstring& _functionName)
{
    if (loadedDep == false && _functionName != L"closeXcos")
    {
        loadOnUseClassPath("Xcos");
        loadedDep = true;
    }

    return 1;
};

int XcosModule::Load()
{
    std::wstring wstModuleName = L"xcos";
#ifdef _MSC_VER
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_2);
#else
    wchar_t* pwstLibName = buildModuleDynLibraryNameW(wstModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(wstModuleName);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].wstFunction, vect[i].wstName, pwstLibName, vect[i].iType, &XcosModule::LoadDeps, wstModuleName));
    }

    FREE(pwstLibName);
    return 1;
}

int MPIModule::Load()
{
    std::wstring wstModuleName = L"mpi";
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

int ExternalObjectsModule::Load()
{
    std::wstring wstModuleName = L"external_objects";
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

int ExternalObjectsJavaModule::Load()
{
    std::wstring wstModuleName = L"external_objects_java";
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

int SlintModule::Load()
{
    std::wstring wstModuleName = L"slint";
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
