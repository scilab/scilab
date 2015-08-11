/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifdef _MSC_VER
#pragma warning(disable : 4996) //It's not beautifull but that works !
#endif


//#include <iostream>
#include <fstream>
//#include <sstream>

//XML API
#include <libxml/xpath.h>
#include <libxml/xmlreader.h>

#include "sci_malloc.h"
#include "funcmanager.hxx"
#include "configvariable.hxx"
#include "module_declaration.hxx"
#include "parser.hxx"
#include "execvisitor.hxx"

extern "C"
{
#include "findfiles.h"
#include "configvariable_interface.h"
#include "os_string.h"
}



using namespace std;
using namespace ast;

#define BASENAMEMODULESFILE L"etc/modules.xml"

bool FileExist(std::string _szFile);
bool FileExist(std::wstring _szFile);
char *GetXmlFileEncoding(string _filename);

FuncManager* FuncManager::me = NULL;

FuncManager* FuncManager::getInstance()
{
    if (me == NULL)
    {
        me = new FuncManager();
        me->CreateModuleList();
        /*get module activation list from xml file*/
        if (me->GetModules() == true)
        {
            if (me->AppendModules() == false)
            {
                destroyInstance();
                return NULL;
            }
        }
        else
        {
            destroyInstance();
            return NULL;
        }
    }

    return me;
}

void FuncManager::destroyInstance()
{
    if (me)
    {
        delete me;
        me = NULL;
    }
}

FuncManager::FuncManager(void)
{
    m_bNoStart = false;
}

FuncManager::~FuncManager(void)
{
}

bool FuncManager::GetModules()
{
    wstring szModulesFilename;

    wstring szPath = ConfigVariable::getSCIPath();
    if (szPath == L"")
    {
        std::wcout << L"The SCI environment variable is not set." << std::endl;
        return false;
    }

    szModulesFilename = szPath + L"/";
    szModulesFilename += BASENAMEMODULESFILE;

    if (FileExist(szModulesFilename))
    {
        m_szXmlFile = szModulesFilename;
    }
    else
    {
        std::wcout << L"Cannot load the module declaration file: " << szModulesFilename << std::endl;
        return false;
    }
    return true;
}

bool FuncManager::AppendModules()
{
    char* pstTemp = wide_string_to_UTF8(m_szXmlFile.c_str());

    char *encoding = GetXmlFileEncoding(pstTemp);

    /* Don't care about line return / empty line */
    xmlKeepBlanksDefault(0);
    /* check if the XML file has been encoded with utf8 (unicode) or not */
    if (stricmp("utf-8", encoding) == 0)
    {
        xmlDocPtr doc;
        xmlXPathContextPtr xpathCtxt	= NULL;
        xmlXPathObjectPtr xpathObj		= NULL;
        char *name										= NULL;
        int activate									= 0;

        doc = xmlParseFile (pstTemp);

        if (doc == NULL)
        {
            std::cout << "Error: Could not parse file " << pstTemp << std::endl;
            if (encoding)
            {
                free(encoding);
                encoding = NULL;
            }
            FREE(pstTemp);
            return false;
        }

        xpathCtxt = xmlXPathNewContext(doc);
        xpathObj = xmlXPathEval((const xmlChar*)"//modules/module", xpathCtxt);

        if (xpathObj && xpathObj->nodesetval->nodeMax)
        {
            /* the Xpath has been understood and there are node */
            for (int	i = 0 ; i < xpathObj->nodesetval->nodeNr ; i++)
            {

                xmlAttrPtr attrib = xpathObj->nodesetval->nodeTab[i]->properties;
                /* Get the properties of <module>  */
                while (attrib != NULL)
                {
                    /* loop until when have read all the attributes */
                    if (xmlStrEqual (attrib->name, (const xmlChar*)"name"))
                    {
                        /* we found the tag name */
                        const char *str = (const char*)attrib->children->content;
                        name = os_strdup(str);
                    }
                    else if (xmlStrEqual(attrib->name, (const xmlChar*)"activate"))
                    {
                        /* we found the tag activate */
                        const char *str = (const char*)attrib->children->content;
                        if (stricmp(str, "yes") == 0 || strcmp(str, "1") == 0)
                        {
                            activate = 1;
                        }
                    }
                    attrib = attrib->next;
                }

                if ((name) && (strlen(name) > 0) && (activate))
                {
                    wchar_t* pstName = to_wide_string(name);
                    if (VerifyModule(pstName))
                    {
                        m_ModuleName.push_back(pstName);
                    }
                    else
                    {
                        std::wcout << pstName << " module not found." << std::endl;
                    }
                    FREE(pstName);
                }

                if (name)
                {
                    free(name);
                    name = NULL;
                }
                activate = 0;
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
        xmlFreeDoc (doc);
    }
    else
    {
        std::cout << "Error: Not a valid module file " << pstTemp << " (encoding not 'utf-8') Encoding '" << encoding << "' found." << std::endl;
    }
    if (encoding)
    {
        free(encoding);
        encoding = NULL;
    }

    ConfigVariable::setModuleList(m_ModuleName);
    FREE(pstTemp);

    return true;
}

bool FuncManager::VerifyModule(wchar_t* _pszModuleName)
{
    wstring SciPath = ConfigVariable::getSCIPath();
    if (SciPath == L"")
    {
        std::wcout << L"The SCI environment variable is not set." << std::endl;
        return false;
    }

    wstring FullPathModuleName = SciPath + L"/modules/" + _pszModuleName + L"/etc/" + _pszModuleName + START_EXT;

    /* ajouter d'autres tests d'existences */

    if (!FileExist(FullPathModuleName))
    {
        return false;
    }
    return true;
}

/*Out of class*/
bool FileExist(std::string _szFile)
{
    bool bReturn = false;
    fstream filestr(_szFile.c_str(), fstream::in);
    bReturn = !filestr.fail();
    filestr.close();
    return bReturn;
}

bool FileExist(std::wstring _szFile)
{
    bool bReturn = false;
    char *pstFile = wide_string_to_UTF8(_szFile.c_str());
    wfstream filestr(pstFile, wfstream::in);
    bReturn = !filestr.fail();
    filestr.close();
    FREE(pstFile);
    return bReturn;
}

char *GetXmlFileEncoding(string _filename)
{
#define DEFAULT_ENCODING "UTF-8"

    char *encoding = NULL;
    xmlDocPtr doc = NULL;

    /* default */
    encoding = os_strdup(DEFAULT_ENCODING);

    doc = xmlParseFile(_filename.c_str());
    if (doc)
    {
        if (doc->encoding)
        {
            if (encoding)
            {
                free(encoding);
                encoding = NULL;
            }
            encoding = os_strdup((char*)doc->encoding);
        }
    }

    xmlFreeDoc(doc);
    return encoding;
}

bool FuncManager::CreateModuleList(void)
{
    m_ModuleMap[L"elementary_functions"] = pair<GW_MOD, GW_MOD>(&ElemFuncModule::Load, &ElemFuncModule::Unload);
    m_ModuleMap[L"types"] = pair<GW_MOD, GW_MOD>(&TypesModule::Load, &TypesModule::Unload);
    m_ModuleMap[L"sparse"] = pair<GW_MOD, GW_MOD>(&SparseModule::Load, &SparseModule::Unload);
    m_ModuleMap[L"boolean"] = pair<GW_MOD, GW_MOD>(&BooleanModule::Load, &BooleanModule::Unload);
    m_ModuleMap[L"integer"] = pair<GW_MOD, GW_MOD>(&IntegerModule::Load, &IntegerModule::Unload);
    m_ModuleMap[L"core"] = pair<GW_MOD, GW_MOD>(&CoreModule::Load, &CoreModule::Unload);
    m_ModuleMap[L"io"] = pair<GW_MOD, GW_MOD>(&IoModule::Load, &IoModule::Unload);
    m_ModuleMap[L"functions"] = pair<GW_MOD, GW_MOD>(&FunctionsModule::Load, &FunctionsModule::Unload);
    m_ModuleMap[L"output_stream"] = pair<GW_MOD, GW_MOD>(&OutputStreamModule::Load, &OutputStreamModule::Unload);
    m_ModuleMap[L"matio"] = pair<GW_MOD, GW_MOD>(&MatioModule::Load, &MatioModule::Unload);
    m_ModuleMap[L"fileio"] = pair<GW_MOD, GW_MOD>(&FileioModule::Load, &FileioModule::Unload);
    m_ModuleMap[L"gui"] = pair<GW_MOD, GW_MOD>(&GuiModule::Load, &GuiModule::Unload);
    m_ModuleMap[L"time"] = pair<GW_MOD, GW_MOD>(&TimeModule::Load, &TimeModule::Unload);
    m_ModuleMap[L"string"] = pair<GW_MOD, GW_MOD>(&StringModule::Load, &StringModule::Unload);
    m_ModuleMap[L"scinotes"] = pair<GW_MOD, GW_MOD>(&ScinotesModule::Load, &ScinotesModule::Unload);
    m_ModuleMap[L"localization"] = pair<GW_MOD, GW_MOD>(&LocalizationModule::Load, &LocalizationModule::Unload);
    m_ModuleMap[L"helptools"] = pair<GW_MOD, GW_MOD>(&HelptoolsModule::Load, &HelptoolsModule::Unload);
    m_ModuleMap[L"hdf5"] = pair<GW_MOD, GW_MOD>(&Hdf5Module::Load, &Hdf5Module::Unload);
    m_ModuleMap[L"dynamic_link"] = pair<GW_MOD, GW_MOD>(&DynamicLinkModule::Load, &DynamicLinkModule::Unload);
    m_ModuleMap[L"action_binding"] = pair<GW_MOD, GW_MOD>(&ActionBindingModule::Load, &ActionBindingModule::Unload);
    m_ModuleMap[L"history_manager"] = pair<GW_MOD, GW_MOD>(&HistoryManagerModule::Load, &HistoryManagerModule::Unload);
    m_ModuleMap[L"console"] = pair<GW_MOD, GW_MOD>(&ConsoleModule::Load, &ConsoleModule::Unload);
    m_ModuleMap[L"signal_processing"] = pair<GW_MOD, GW_MOD>(&SignalProcessingModule::Load, &SignalProcessingModule::Unload);
    m_ModuleMap[L"linear_algebra"] = pair<GW_MOD, GW_MOD>(&LinearAlgebraModule::Load, &LinearAlgebraModule::Unload);
    m_ModuleMap[L"statistics"] = pair<GW_MOD, GW_MOD>(&StatisticsModule::Load, &StatisticsModule::Unload);
    m_ModuleMap[L"differential_equations"] = pair<GW_MOD, GW_MOD>(&DifferentialEquationsModule::Load, &DifferentialEquationsModule::Unload);
    m_ModuleMap[L"cacsd"] = pair<GW_MOD, GW_MOD>(&CacsdModule::Load, &CacsdModule::Unload);
    m_ModuleMap[L"spreadsheet"] = pair<GW_MOD, GW_MOD>(&SpreadsheetModule::Load, &SpreadsheetModule::Unload);
    m_ModuleMap[L"randlib"] = pair<GW_MOD, GW_MOD>(&RandlibModule::Load, &RandlibModule::Unload);
    m_ModuleMap[L"graphics"] = pair<GW_MOD, GW_MOD>(&GraphicsModule::Load, &GraphicsModule::Unload);
    m_ModuleMap[L"interpolation"] = pair<GW_MOD, GW_MOD>(&InterpolationModule::Load, &InterpolationModule::Unload);
    m_ModuleMap[L"sound"] = pair<GW_MOD, GW_MOD>(&SoundModule::Load, &SoundModule::Unload);
    m_ModuleMap[L"umfpack"] = pair<GW_MOD, GW_MOD>(&UmfpackModule::Load, &UmfpackModule::Unload);
    m_ModuleMap[L"optimization"] = pair<GW_MOD, GW_MOD>(&OptimizationModule::Load, &OptimizationModule::Unload);
    m_ModuleMap[L"special_functions"] = pair<GW_MOD, GW_MOD>(&SpecialFunctionsModule::Load, &SpecialFunctionsModule::Unload);
    m_ModuleMap[L"graphic_export"] = pair<GW_MOD, GW_MOD>(&GraphicExportModule::Load, &GraphicExportModule::Unload);
    m_ModuleMap[L"polynomials"] = pair<GW_MOD, GW_MOD>(&PolynomialsModule::Load, &PolynomialsModule::Unload);
    m_ModuleMap[L"arnoldi"] = pair<GW_MOD, GW_MOD>(&ArnoldiModule::Load, &ArnoldiModule::Unload);
    m_ModuleMap[L"data_structures"] = pair<GW_MOD, GW_MOD>(&DataStructuresModule::Load, &DataStructuresModule::Unload);
    m_ModuleMap[L"call_scilab"] = pair<GW_MOD, GW_MOD>(&CallScilabModule::Load, &CallScilabModule::Unload);
    m_ModuleMap[L"completion"] = pair<GW_MOD, GW_MOD>(&CompletionModule::Load, &CompletionModule::Unload);
    m_ModuleMap[L"xml"] = pair<GW_MOD, GW_MOD>(&XmlModule::Load, &XmlModule::Unload);
    m_ModuleMap[L"scicos"] = pair<GW_MOD, GW_MOD>(&ScicosModule::Load, &ScicosModule::Unload);
    m_ModuleMap[L"xcos"] = pair<GW_MOD, GW_MOD>(&XcosModule::Load, &XcosModule::Unload);
    m_ModuleMap[L"fftw"] = pair<GW_MOD, GW_MOD>(&FFTWModule::Load, &FFTWModule::Unload);
    m_ModuleMap[L"mpi"] = pair<GW_MOD, GW_MOD>(&MPIModule::Load, &MPIModule::Unload);
    m_ModuleMap[L"external_objects"] = pair<GW_MOD, GW_MOD>(&ExternalObjectsModule::Load, &ExternalObjectsModule::Unload);
    m_ModuleMap[L"external_objects_java"] = pair<GW_MOD, GW_MOD>(&ExternalObjectsJavaModule::Load, &ExternalObjectsJavaModule::Unload);
    m_ModuleMap[L"preferences"] = pair<GW_MOD, GW_MOD>(&PreferencesModule::Load, &PreferencesModule::Unload);

    if (ConfigVariable::getScilabMode() != SCILAB_NWNI)
    {
        m_ModuleMap[L"jvm"] = pair<GW_MOD, GW_MOD>(&JvmModule::Load, &JvmModule::Unload);
        m_ModuleMap[L"ui_data"] = pair<GW_MOD, GW_MOD>(&UiDataModule::Load, &UiDataModule::Unload);
    }
#ifdef _MSC_VER
    m_ModuleMap[L"windows_tools"] = pair<GW_MOD, GW_MOD>(&WindowsToolsModule::Load, &WindowsToolsModule::Unload);
#endif
    return true;
}

bool FuncManager::ExecuteFile(wstring _stFile)
{
    Parser parser;

    parser.parseFile(_stFile, ConfigVariable::getSCIPath());

    if (parser.getExitStatus() == Parser::Failed)
    {
        std::wostringstream ostr;
        ostr << _W("Unable to execute : ") << _stFile << endl;
        scilabWriteW(ostr.str().c_str());
        delete parser.getTree();
        return false;
    }

    ExecVisitor exec;

    //save current prompt mode
    int oldVal = ConfigVariable::getPromptMode();
    //set mode silent for errors
    ConfigVariable::setPromptMode(-1);
    try
    {
        parser.getTree()->accept(exec);
    }
    catch (const ast::InternalError& ie)
    {
        scilabWriteW(ie.GetErrorMessage().c_str());
    }

    //restore previous prompt mode
    ConfigVariable::setPromptMode(oldVal);
    delete parser.getTree();
    return true;
}

bool FuncManager::LoadModules()
{
    list<wstring>::const_iterator it = m_ModuleName.begin();
    list<wstring>::const_iterator itEnd = m_ModuleName.end();
    //load gateways
    for (; it != itEnd; ++it)
    {
        ModuleMap::iterator itModule = m_ModuleMap.find(*it);
        if (itModule != m_ModuleMap.end())
        {
            //call ::Load function
            itModule->second.first();
        }
    }

    return true;
}

bool FuncManager::EndModules()
{
    list<wstring>::const_iterator it = m_ModuleName.begin();
    list<wstring>::const_iterator itEnd = m_ModuleName.end();
    //excute .start file
    for (; it != itEnd; ++it)
    {
        ExecuteQuitFile(*it);
    }

    return true;
}


bool FuncManager::UnloadModules()
{
    list<wstring>::const_iterator it = m_ModuleName.begin();
    list<wstring>::const_iterator itEnd = m_ModuleName.end();
    //load gateways
    for (; it != itEnd; ++it)
    {
        ModuleMap::iterator itModule = m_ModuleMap.find(*it);
        if (itModule != m_ModuleMap.end())
        {
            //call ::Unload function
            itModule->second.second();
        }
    }

    //Unload shared libraries
    int iCount = ConfigVariable::getDynModuleCount();
    DynLibHandle* libs = ConfigVariable::getAllDynModule();
    for (int i = 0 ; i < iCount ; i++)
    {
        FreeDynLibrary(libs[i]);
    }

    ConfigVariable::cleanDynModule();
    delete[] libs;
    return true;
}

bool FuncManager::ExecuteStartFile(wstring _stModule)
{
    //build .start filename
    wstring stPath = ConfigVariable::getSCIPath();
    stPath += MODULE_DIR;
    stPath += _stModule;
    stPath += ETC_DIR;
    stPath += _stModule;
    stPath += START_EXT;

    return ExecuteFile(stPath);
}

bool FuncManager::ExecuteQuitFile(wstring _stModule)
{
    //build .quit filename
    wstring stPath = ConfigVariable::getSCIPath();
    stPath += MODULE_DIR;
    stPath += _stModule;
    stPath += ETC_DIR;
    stPath += _stModule;
    stPath += QUIT_EXT;

    return ExecuteFile(stPath);
}
