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

vectGateway loadGatewaysName(const std::string& _stModuleName)
{
    vectGateway vect;
    std::string stPath = ConfigVariable::getSCIPath();
    std::string stModuleName = stPath + "/modules/" + _stModuleName + "/sci_gateway/" + _stModuleName + "_gateway.xml";

    /* Don't care about line return / empty line */
    xmlKeepBlanksDefault(0);
    // parse file
    xmlDocPtr doc;
    xmlXPathContextPtr xpathCtxt = NULL;
    xmlXPathObjectPtr xpathObj = NULL;

    doc = xmlParseFile(stModuleName.c_str());
    if (doc == NULL)
    {
        std::cout << "Error: Could not parse file " << stModuleName.c_str() << std::endl;
        return vect;
    }

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
                    str.stName = (const char*)attrib->children->content;
                }
                else if (xmlStrEqual(attrib->name, (const xmlChar*)"function"))
                {
                    str.stFunction = (const char*)attrib->children->content;
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
int ScinotesModule::LoadDeps(const std::string& _functionName)
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
    std::string stModuleName = "scinotes";
#ifdef _MSC_VER
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_2);
#else
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(stModuleName);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].stFunction, vect[i].stName, pstLibName, vect[i].iType, &ScinotesModule::LoadDeps, stModuleName));
    }

    FREE(pstLibName);
    return 1;
}

//Functions module
int FunctionsModule::Load()
{
    std::string stPath = "functions";
#ifdef _MSC_VER
    std::string stModuleName = "functions_gw";
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    std::string stModuleName = "functions";
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(stPath);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].stFunction, vect[i].stName, pstLibName, vect[i].iType, NULL, stModuleName));
    }

    FREE(pstLibName);
    return 1;
}

int StatisticsModule::Load()
{
    std::string stPath = "statistics";
#ifdef _MSC_VER
    std::string stModuleName = "statistics_gw";
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    std::string stModuleName = "statistics";
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(stPath);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].stFunction, vect[i].stName, pstLibName, vect[i].iType, NULL, stModuleName));
    }

    FREE(pstLibName);
    return 1;
}

int SignalProcessingModule::Load()
{
    std::string stPath = "signal_processing";
#ifdef _MSC_VER
    std::string stModuleName = "signal_processing_gw";
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    std::string stModuleName = "signal_processing";
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(stPath);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].stFunction, vect[i].stName, pstLibName, vect[i].iType, NULL, stModuleName));
    }

    FREE(pstLibName);
    return 1;
}

int HelptoolsModule::LoadDeps(const std::string& _functionName)
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
    std::string stModuleName = "helptools";
#ifdef _MSC_VER
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif

    vectGateway vect = loadGatewaysName(stModuleName);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].stFunction, vect[i].stName, pstLibName, vect[i].iType, &HelptoolsModule::LoadDeps, stModuleName));
    }

    FREE(pstLibName);
    return 1;
}

int MatioModule::Load()
{
    std::string stPath = "matio";
#ifdef _MSC_VER
    std::string stModuleName = "matio_gw";
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    std::string stModuleName = "matio";
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(stPath);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].stFunction, vect[i].stName, pstLibName, vect[i].iType, NULL, stModuleName));
    }

    FREE(pstLibName);
    return 1;
}

int Hdf5Module::Load()
{
    std::string stModuleName = "hdf5";
#ifdef _MSC_VER
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(stModuleName);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].stFunction, vect[i].stName, pstLibName, vect[i].iType, NULL, stModuleName));
    }

    FREE(pstLibName);
    return 1;
}

int ActionBindingModule::Load()
{
    std::string stPath = "action_binding";
#ifdef _MSC_VER
    std::string stModuleName = "action_binding_gw";
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_2);
#else
    std::string stModuleName = "action_binding";
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(stPath);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].stFunction, vect[i].stName, pstLibName, vect[i].iType, NULL, stModuleName));
    }

    FREE(pstLibName);
    return 1;
}

int SpreadsheetModule::Load()
{
    std::string stModuleName = "spreadsheet";
#ifdef _MSC_VER
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(stModuleName);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].stFunction, vect[i].stName, pstLibName, vect[i].iType, NULL, stModuleName));
    }

    FREE(pstLibName);
    return 1;
}

int InterpolationModule::Load()
{
    std::string stPath = "interpolation";
#ifdef _MSC_VER
    std::string stModuleName = "interpolation";
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    std::string stModuleName = "interpolation";
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(stPath);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].stFunction, vect[i].stName, pstLibName, vect[i].iType, NULL, stModuleName));
    }

    FREE(pstLibName);
    return 1;
}

int SoundModule::Load()
{
    std::string stPath = "sound";
#ifdef _MSC_VER
    std::string stModuleName = "sound";
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    std::string stModuleName = "sound";
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(stPath);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].stFunction, vect[i].stName, pstLibName, vect[i].iType, NULL, stModuleName));
    }

    FREE(pstLibName);
    return 1;
}

int RandlibModule::Load()
{
    std::string stModuleName = "randlib";
#ifdef _MSC_VER
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(stModuleName);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].stFunction, vect[i].stName, pstLibName, vect[i].iType, NULL, stModuleName));
    }

    FREE(pstLibName);
    return 1;
}

int UmfpackModule::Load()
{
    std::string stModuleName = "umfpack";
#ifdef _MSC_VER
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(stModuleName);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].stFunction, vect[i].stName, pstLibName, vect[i].iType, NULL, stModuleName));
    }

    FREE(pstLibName);
    return 1;
}

int OptimizationModule::Load()
{
    std::string stModuleName = "optimization";
#ifdef _MSC_VER
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(stModuleName);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].stFunction, vect[i].stName, pstLibName, vect[i].iType, NULL, stModuleName));
    }

    FREE(pstLibName);
    return 1;
}

int SpecialFunctionsModule::Load()
{
    std::string stModuleName = "special_functions";
#ifdef _MSC_VER
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(stModuleName);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].stFunction, vect[i].stName, pstLibName, vect[i].iType, NULL, stModuleName));
    }

    FREE(pstLibName);
    return 1;
}

int GraphicExportModule::Load()
{
    std::string stModuleName = "graphic_export";
#ifdef _MSC_VER
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_2);
#else
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(stModuleName);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].stFunction, vect[i].stName, pstLibName, vect[i].iType, NULL, stModuleName));
    }

    FREE(pstLibName);
    return 1;
}

int ArnoldiModule::Load()
{
    std::string stModuleName = "arnoldi";
#ifdef _MSC_VER
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(stModuleName);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].stFunction, vect[i].stName, pstLibName, vect[i].iType, NULL, stModuleName));
    }

    FREE(pstLibName);
    return 1;
}

int CallScilabModule::Load()
{
    std::string stModuleName = "call_scilab";
#ifdef _MSC_VER
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(stModuleName);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].stFunction, vect[i].stName, pstLibName, vect[i].iType, NULL, stModuleName));
    }

    FREE(pstLibName);
    return 1;
}

int CompletionModule::Load()
{
    std::string stModuleName = "completion";
#ifdef _MSC_VER
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_2);
#else
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(stModuleName);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].stFunction, vect[i].stName, pstLibName, vect[i].iType, NULL, stModuleName));
    }

    FREE(pstLibName);
    return 1;
}

int XmlModule::Load()
{
    std::string stModuleName = "xml";
#ifdef _MSC_VER
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(stModuleName);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].stFunction, vect[i].stName, pstLibName, vect[i].iType, NULL, stModuleName));
    }

    FREE(pstLibName);
    return 1;
}

int ScicosModule::Load()
{
    std::string stModuleName = "scicos";
    const char* stLibName = stModuleName.c_str();
    if (getScilabMode() == SCILAB_NWNI)
    {
        stLibName = "scicos-cli";
    }
#ifdef _MSC_VER
    char* pstLibName = buildModuleDynLibraryName(stLibName, DYNLIB_NAME_FORMAT_1);
#else
    char* pstLibName = buildModuleDynLibraryName(stLibName, DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(stModuleName);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].stFunction, vect[i].stName, pstLibName, vect[i].iType, NULL, stModuleName));
    }

    FREE(pstLibName);
    return 1;
}

bool XcosModule::loadedDep = false;
int XcosModule::LoadDeps(const std::string& _functionName)
{
    if (loadedDep == false && _functionName != "closeXcos")
    {
        loadOnUseClassPath("Xcos");
        loadedDep = true;
    }

    return 1;
};

int XcosModule::Load()
{
    std::string stModuleName = "xcos";
#ifdef _MSC_VER
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_2);
#else
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(stModuleName);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].stFunction, vect[i].stName, pstLibName, vect[i].iType, &XcosModule::LoadDeps, stModuleName));
    }

    FREE(pstLibName);
    return 1;
}

int MPIModule::Load()
{
    std::string stModuleName = "mpi";
#ifdef _MSC_VER
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(stModuleName);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].stFunction, vect[i].stName, pstLibName, vect[i].iType, NULL, stModuleName));
    }

    FREE(pstLibName);
    return 1;
}

int ExternalObjectsModule::Load()
{
    std::string stModuleName = "external_objects";
#ifdef _MSC_VER
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(stModuleName);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].stFunction, vect[i].stName, pstLibName, vect[i].iType, NULL, stModuleName));
    }

    FREE(pstLibName);
    return 1;
}

int ExternalObjectsJavaModule::Load()
{
    std::string stModuleName = "external_objects_java";
#ifdef _MSC_VER
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(stModuleName);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].stFunction, vect[i].stName, pstLibName, vect[i].iType, NULL, stModuleName));
    }

    FREE(pstLibName);
    return 1;
}

int SlintModule::Load()
{
    std::string stModuleName = "slint";
#ifdef _MSC_VER
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_1);
#else
    char* pstLibName = buildModuleDynLibraryName(stModuleName.c_str(), DYNLIB_NAME_FORMAT_3);
#endif
    vectGateway vect = loadGatewaysName(stModuleName);

    for (int i = 0 ; i < (int)vect.size() ; i++)
    {
        symbol::Context::getInstance()->addFunction(types::Function::createFunction(vect[i].stFunction, vect[i].stName, pstLibName, vect[i].iType, NULL, stModuleName));
    }

    FREE(pstLibName);
    return 1;
}
