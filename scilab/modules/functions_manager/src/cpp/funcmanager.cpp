/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
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
#include "execvisitor.hxx"
#include "configvariable.hxx"
#include "module_declaration.hxx"
#include "parser.hxx"

extern "C"
{
#include "findfiles.h"
#include "configvariable_interface.h"
#include "os_string.h"
}

#define BASENAMEMODULESFILE L"etc/modules.xml"

bool FileExist(std::string _szFile);
bool FileExist(std::wstring _szFile);
char *GetXmlFileEncoding(const std::string& _filename);

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
    std::wstring szModulesFilename;

    std::wstring szPath = ConfigVariable::getSCIPath();
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
            free(encoding);
            encoding = NULL;
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
                        if (name)
                        {
                            FREE(name);
                        }
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
    free(encoding);
    encoding = NULL;

    ConfigVariable::setModuleList(m_ModuleName);
    FREE(pstTemp);

    return true;
}

bool FuncManager::VerifyModule(wchar_t* _pszModuleName)
{
    std::wstring SciPath = ConfigVariable::getSCIPath();
    if (SciPath == L"")
    {
        std::wcout << L"The SCI environment variable is not set." << std::endl;
        return false;
    }

    std::wstring FullPathModuleName = SciPath + L"/modules/" + _pszModuleName + L"/etc/" + _pszModuleName + START_EXT;

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
    std::fstream filestr(_szFile.c_str(), std::fstream::in);
    bReturn = !filestr.fail();
    filestr.close();
    return bReturn;
}

bool FileExist(std::wstring _szFile)
{
    bool bReturn = false;
    char *pstFile = wide_string_to_UTF8(_szFile.c_str());
    std::wfstream filestr(pstFile, std::wfstream::in);
    bReturn = !filestr.fail();
    filestr.close();
    FREE(pstFile);
    return bReturn;
}

char *GetXmlFileEncoding(const std::string& _filename)
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
    m_ModuleMap[L"elementary_functions"] = std::pair<GW_MOD, GW_MOD>(&ElemFuncModule::Load, &ElemFuncModule::Unload);
    m_ModuleMap[L"sparse"] = std::pair<GW_MOD, GW_MOD>(&SparseModule::Load, &SparseModule::Unload);
    m_ModuleMap[L"boolean"] = std::pair<GW_MOD, GW_MOD>(&BooleanModule::Load, &BooleanModule::Unload);
    m_ModuleMap[L"integer"] = std::pair<GW_MOD, GW_MOD>(&IntegerModule::Load, &IntegerModule::Unload);
    m_ModuleMap[L"core"] = std::pair<GW_MOD, GW_MOD>(&CoreModule::Load, &CoreModule::Unload);
    m_ModuleMap[L"io"] = std::pair<GW_MOD, GW_MOD>(&IoModule::Load, &IoModule::Unload);
    m_ModuleMap[L"functions"] = std::pair<GW_MOD, GW_MOD>(&FunctionsModule::Load, &FunctionsModule::Unload);
    m_ModuleMap[L"output_stream"] = std::pair<GW_MOD, GW_MOD>(&OutputStreamModule::Load, &OutputStreamModule::Unload);
    m_ModuleMap[L"matio"] = std::pair<GW_MOD, GW_MOD>(&MatioModule::Load, &MatioModule::Unload);
    m_ModuleMap[L"fileio"] = std::pair<GW_MOD, GW_MOD>(&FileioModule::Load, &FileioModule::Unload);
    m_ModuleMap[L"gui"] = std::pair<GW_MOD, GW_MOD>(&GuiModule::Load, &GuiModule::Unload);
    m_ModuleMap[L"time"] = std::pair<GW_MOD, GW_MOD>(&TimeModule::Load, &TimeModule::Unload);
    m_ModuleMap[L"string"] = std::pair<GW_MOD, GW_MOD>(&StringModule::Load, &StringModule::Unload);
    m_ModuleMap[L"scinotes"] = std::pair<GW_MOD, GW_MOD>(&ScinotesModule::Load, &ScinotesModule::Unload);
    m_ModuleMap[L"localization"] = std::pair<GW_MOD, GW_MOD>(&LocalizationModule::Load, &LocalizationModule::Unload);
    m_ModuleMap[L"helptools"] = std::pair<GW_MOD, GW_MOD>(&HelptoolsModule::Load, &HelptoolsModule::Unload);
    m_ModuleMap[L"hdf5"] = std::pair<GW_MOD, GW_MOD>(&Hdf5Module::Load, &Hdf5Module::Unload);
    m_ModuleMap[L"dynamic_link"] = std::pair<GW_MOD, GW_MOD>(&DynamicLinkModule::Load, &DynamicLinkModule::Unload);
    m_ModuleMap[L"action_binding"] = std::pair<GW_MOD, GW_MOD>(&ActionBindingModule::Load, &ActionBindingModule::Unload);
    m_ModuleMap[L"history_manager"] = std::pair<GW_MOD, GW_MOD>(&HistoryManagerModule::Load, &HistoryManagerModule::Unload);
    m_ModuleMap[L"console"] = std::pair<GW_MOD, GW_MOD>(&ConsoleModule::Load, &ConsoleModule::Unload);
    m_ModuleMap[L"signal_processing"] = std::pair<GW_MOD, GW_MOD>(&SignalProcessingModule::Load, &SignalProcessingModule::Unload);
    m_ModuleMap[L"linear_algebra"] = std::pair<GW_MOD, GW_MOD>(&LinearAlgebraModule::Load, &LinearAlgebraModule::Unload);
    m_ModuleMap[L"statistics"] = std::pair<GW_MOD, GW_MOD>(&StatisticsModule::Load, &StatisticsModule::Unload);
    m_ModuleMap[L"differential_equations"] = std::pair<GW_MOD, GW_MOD>(&DifferentialEquationsModule::Load, &DifferentialEquationsModule::Unload);
    m_ModuleMap[L"cacsd"] = std::pair<GW_MOD, GW_MOD>(&CacsdModule::Load, &CacsdModule::Unload);
    m_ModuleMap[L"spreadsheet"] = std::pair<GW_MOD, GW_MOD>(&SpreadsheetModule::Load, &SpreadsheetModule::Unload);
    m_ModuleMap[L"randlib"] = std::pair<GW_MOD, GW_MOD>(&RandlibModule::Load, &RandlibModule::Unload);
    m_ModuleMap[L"graphics"] = std::pair<GW_MOD, GW_MOD>(&GraphicsModule::Load, &GraphicsModule::Unload);
    m_ModuleMap[L"interpolation"] = std::pair<GW_MOD, GW_MOD>(&InterpolationModule::Load, &InterpolationModule::Unload);
    m_ModuleMap[L"sound"] = std::pair<GW_MOD, GW_MOD>(&SoundModule::Load, &SoundModule::Unload);
    m_ModuleMap[L"umfpack"] = std::pair<GW_MOD, GW_MOD>(&UmfpackModule::Load, &UmfpackModule::Unload);
    m_ModuleMap[L"optimization"] = std::pair<GW_MOD, GW_MOD>(&OptimizationModule::Load, &OptimizationModule::Unload);
    m_ModuleMap[L"special_functions"] = std::pair<GW_MOD, GW_MOD>(&SpecialFunctionsModule::Load, &SpecialFunctionsModule::Unload);
    m_ModuleMap[L"graphic_export"] = std::pair<GW_MOD, GW_MOD>(&GraphicExportModule::Load, &GraphicExportModule::Unload);
    m_ModuleMap[L"polynomials"] = std::pair<GW_MOD, GW_MOD>(&PolynomialsModule::Load, &PolynomialsModule::Unload);
    m_ModuleMap[L"arnoldi"] = std::pair<GW_MOD, GW_MOD>(&ArnoldiModule::Load, &ArnoldiModule::Unload);
    m_ModuleMap[L"data_structures"] = std::pair<GW_MOD, GW_MOD>(&DataStructuresModule::Load, &DataStructuresModule::Unload);
    m_ModuleMap[L"call_scilab"] = std::pair<GW_MOD, GW_MOD>(&CallScilabModule::Load, &CallScilabModule::Unload);
    m_ModuleMap[L"completion"] = std::pair<GW_MOD, GW_MOD>(&CompletionModule::Load, &CompletionModule::Unload);
    m_ModuleMap[L"xml"] = std::pair<GW_MOD, GW_MOD>(&XmlModule::Load, &XmlModule::Unload);
    m_ModuleMap[L"scicos"] = std::pair<GW_MOD, GW_MOD>(&ScicosModule::Load, &ScicosModule::Unload);
    m_ModuleMap[L"xcos"] = std::pair<GW_MOD, GW_MOD>(&XcosModule::Load, &XcosModule::Unload);
    m_ModuleMap[L"fftw"] = std::pair<GW_MOD, GW_MOD>(&FFTWModule::Load, &FFTWModule::Unload);
    m_ModuleMap[L"mpi"] = std::pair<GW_MOD, GW_MOD>(&MPIModule::Load, &MPIModule::Unload);
    m_ModuleMap[L"external_objects"] = std::pair<GW_MOD, GW_MOD>(&ExternalObjectsModule::Load, &ExternalObjectsModule::Unload);
    m_ModuleMap[L"external_objects_java"] = std::pair<GW_MOD, GW_MOD>(&ExternalObjectsJavaModule::Load, &ExternalObjectsJavaModule::Unload);
    m_ModuleMap[L"preferences"] = std::pair<GW_MOD, GW_MOD>(&PreferencesModule::Load, &PreferencesModule::Unload);
    m_ModuleMap[L"slint"] = std::pair<GW_MOD, GW_MOD>(&SlintModule::Load, &SlintModule::Unload);
    m_ModuleMap[L"coverage"] = std::pair<GW_MOD, GW_MOD>(&CoverageModule::Load, &SlintModule::Unload);

    if (ConfigVariable::getScilabMode() != SCILAB_NWNI)
    {
        m_ModuleMap[L"tclsci"] = std::pair<GW_MOD, GW_MOD>(&TclsciModule::Load, &TclsciModule::Unload);
        m_ModuleMap[L"jvm"] = std::pair<GW_MOD, GW_MOD>(&JvmModule::Load, &JvmModule::Unload);
        m_ModuleMap[L"ui_data"] = std::pair<GW_MOD, GW_MOD>(&UiDataModule::Load, &UiDataModule::Unload);
    }
#ifdef _MSC_VER
    m_ModuleMap[L"windows_tools"] = std::pair<GW_MOD, GW_MOD>(&WindowsToolsModule::Load, &WindowsToolsModule::Unload);
#endif
    return true;
}

bool FuncManager::ExecuteFile(const std::wstring& _stFile)
{
    Parser parser;

    parser.parseFile(_stFile, ConfigVariable::getSCIPath());

    if (parser.getExitStatus() == Parser::Failed)
    {
        std::wostringstream ostr;
        ostr << _W("Unable to execute : ") << _stFile << std::endl;
        scilabWriteW(ostr.str().c_str());
        delete parser.getTree();
        return false;
    }

    //save current prompt mode
    int oldVal = ConfigVariable::getPromptMode();
    //set mode silent for errors
    ConfigVariable::setPromptMode(-1);
    try
    {
        ast::ExecVisitor exec;
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
    //load gateways
    for (const auto & it : m_ModuleName)
    {
        ModuleMap::iterator itModule = m_ModuleMap.find(it);
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
    //excute .start file
    for (const auto & it : m_ModuleName)
    {
        ExecuteQuitFile(it);
    }

    return true;
}


bool FuncManager::UnloadModules()
{
    //load gateways
    for (const auto & it : m_ModuleName)
    {
        ModuleMap::iterator itModule = m_ModuleMap.find(it);
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

bool FuncManager::ExecuteStartFile(const std::wstring& _stModule)
{
    //build .start filename
    std::wstring stPath = ConfigVariable::getSCIPath();
    stPath += MODULE_DIR;
    stPath += _stModule;
    stPath += ETC_DIR;
    stPath += _stModule;
    stPath += START_EXT;

    return ExecuteFile(stPath);
}

bool FuncManager::ExecuteQuitFile(const std::wstring& _stModule)
{
    //build .quit filename
    std::wstring stPath = ConfigVariable::getSCIPath();
    stPath += MODULE_DIR;
    stPath += _stModule;
    stPath += ETC_DIR;
    stPath += _stModule;
    stPath += QUIT_EXT;

    return ExecuteFile(stPath);
}
