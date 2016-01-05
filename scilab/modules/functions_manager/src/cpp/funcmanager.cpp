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

#define BASENAMEMODULESFILE "etc/modules.xml"

bool FileExist(const std::string& _szFile);
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
    std::string szModulesFilename;

    std::string szPath = ConfigVariable::getSCIPath();
    if (szPath == "")
    {
        std::cout << "The SCI environment variable is not set." << std::endl;
        return false;
    }

    szModulesFilename = szPath + "/";
    szModulesFilename += BASENAMEMODULESFILE;

    if (FileExist(szModulesFilename))
    {
        m_szXmlFile = szModulesFilename;
    }
    else
    {
        std::cout << "Cannot load the module declaration file: " << szModulesFilename << std::endl;
        return false;
    }
    return true;
}

bool FuncManager::AppendModules()
{
    char *encoding = GetXmlFileEncoding(m_szXmlFile.c_str());

    /* Don't care about line return / empty line */
    xmlKeepBlanksDefault(0);
    /* check if the XML file has been encoded with utf8 (unicode) or not */
    if (stricmp("utf-8", encoding) == 0)
    {
        xmlDocPtr doc;
        xmlXPathContextPtr xpathCtxt = NULL;
        xmlXPathObjectPtr xpathObj = NULL;
        char *name = NULL;
        int activate = 0;

        doc = xmlParseFile(m_szXmlFile.c_str());

        if (doc == NULL)
        {
            std::cout << "Error: Could not parse file " << m_szXmlFile.c_str() << std::endl;
            if (encoding)
            {
                free(encoding);
                encoding = NULL;
            }
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
                    if (VerifyModule(name))
                    {
                        m_ModuleName.push_back(name);
                    }
                    else
                    {
                        std::cout << name << " module not found." << std::endl;
                    }
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
        std::cout << "Error: Not a valid module file " << m_szXmlFile.c_str() << " (encoding not 'utf-8') Encoding '" << encoding << "' found." << std::endl;
    }
    if (encoding)
    {
        free(encoding);
        encoding = NULL;
    }

    ConfigVariable::setModuleList(m_ModuleName);

    return true;
}

bool FuncManager::VerifyModule(const char* _moduleName)
{
    std::string SciPath = ConfigVariable::getSCIPath();
    if (SciPath == "")
    {
        std::cout << "The SCI environment variable is not set." << std::endl;
        return false;
    }

    std::string FullPathModuleName = SciPath + "/modules/" + _moduleName + "/etc/" + _moduleName + START_EXT;

    /* ajouter d'autres tests d'existences */

    if (!FileExist(FullPathModuleName))
    {
        return false;
    }
    return true;
}

/*Out of class*/
bool FileExist(const std::string& _szFile)
{
    bool bReturn = false;
    std::fstream filestr(_szFile.c_str(), std::fstream::in);
    bReturn = !filestr.fail();
    filestr.close();
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
    m_ModuleMap["elementary_functions"] = std::pair<GW_MOD, GW_MOD>(&ElemFuncModule::Load, &ElemFuncModule::Unload);
    m_ModuleMap["sparse"] = std::pair<GW_MOD, GW_MOD>(&SparseModule::Load, &SparseModule::Unload);
    m_ModuleMap["boolean"] = std::pair<GW_MOD, GW_MOD>(&BooleanModule::Load, &BooleanModule::Unload);
    m_ModuleMap["integer"] = std::pair<GW_MOD, GW_MOD>(&IntegerModule::Load, &IntegerModule::Unload);
    m_ModuleMap["core"] = std::pair<GW_MOD, GW_MOD>(&CoreModule::Load, &CoreModule::Unload);
    m_ModuleMap["io"] = std::pair<GW_MOD, GW_MOD>(&IoModule::Load, &IoModule::Unload);
    m_ModuleMap["functions"] = std::pair<GW_MOD, GW_MOD>(&FunctionsModule::Load, &FunctionsModule::Unload);
    m_ModuleMap["output_stream"] = std::pair<GW_MOD, GW_MOD>(&OutputStreamModule::Load, &OutputStreamModule::Unload);
    m_ModuleMap["matio"] = std::pair<GW_MOD, GW_MOD>(&MatioModule::Load, &MatioModule::Unload);
    m_ModuleMap["fileio"] = std::pair<GW_MOD, GW_MOD>(&FileioModule::Load, &FileioModule::Unload);
    m_ModuleMap["gui"] = std::pair<GW_MOD, GW_MOD>(&GuiModule::Load, &GuiModule::Unload);
    m_ModuleMap["time"] = std::pair<GW_MOD, GW_MOD>(&TimeModule::Load, &TimeModule::Unload);
    m_ModuleMap["string"] = std::pair<GW_MOD, GW_MOD>(&StringModule::Load, &StringModule::Unload);
    m_ModuleMap["scinotes"] = std::pair<GW_MOD, GW_MOD>(&ScinotesModule::Load, &ScinotesModule::Unload);
    m_ModuleMap["localization"] = std::pair<GW_MOD, GW_MOD>(&LocalizationModule::Load, &LocalizationModule::Unload);
    m_ModuleMap["helptools"] = std::pair<GW_MOD, GW_MOD>(&HelptoolsModule::Load, &HelptoolsModule::Unload);
    m_ModuleMap["hdf5"] = std::pair<GW_MOD, GW_MOD>(&Hdf5Module::Load, &Hdf5Module::Unload);
    m_ModuleMap["dynamic_link"] = std::pair<GW_MOD, GW_MOD>(&DynamicLinkModule::Load, &DynamicLinkModule::Unload);
    m_ModuleMap["action_binding"] = std::pair<GW_MOD, GW_MOD>(&ActionBindingModule::Load, &ActionBindingModule::Unload);
    m_ModuleMap["history_manager"] = std::pair<GW_MOD, GW_MOD>(&HistoryManagerModule::Load, &HistoryManagerModule::Unload);
    m_ModuleMap["console"] = std::pair<GW_MOD, GW_MOD>(&ConsoleModule::Load, &ConsoleModule::Unload);
    m_ModuleMap["signal_processing"] = std::pair<GW_MOD, GW_MOD>(&SignalProcessingModule::Load, &SignalProcessingModule::Unload);
    m_ModuleMap["linear_algebra"] = std::pair<GW_MOD, GW_MOD>(&LinearAlgebraModule::Load, &LinearAlgebraModule::Unload);
    m_ModuleMap["statistics"] = std::pair<GW_MOD, GW_MOD>(&StatisticsModule::Load, &StatisticsModule::Unload);
    m_ModuleMap["differential_equations"] = std::pair<GW_MOD, GW_MOD>(&DifferentialEquationsModule::Load, &DifferentialEquationsModule::Unload);
    m_ModuleMap["cacsd"] = std::pair<GW_MOD, GW_MOD>(&CacsdModule::Load, &CacsdModule::Unload);
    m_ModuleMap["spreadsheet"] = std::pair<GW_MOD, GW_MOD>(&SpreadsheetModule::Load, &SpreadsheetModule::Unload);
    m_ModuleMap["randlib"] = std::pair<GW_MOD, GW_MOD>(&RandlibModule::Load, &RandlibModule::Unload);
    m_ModuleMap["graphics"] = std::pair<GW_MOD, GW_MOD>(&GraphicsModule::Load, &GraphicsModule::Unload);
    m_ModuleMap["interpolation"] = std::pair<GW_MOD, GW_MOD>(&InterpolationModule::Load, &InterpolationModule::Unload);
    m_ModuleMap["sound"] = std::pair<GW_MOD, GW_MOD>(&SoundModule::Load, &SoundModule::Unload);
    m_ModuleMap["umfpack"] = std::pair<GW_MOD, GW_MOD>(&UmfpackModule::Load, &UmfpackModule::Unload);
    m_ModuleMap["optimization"] = std::pair<GW_MOD, GW_MOD>(&OptimizationModule::Load, &OptimizationModule::Unload);
    m_ModuleMap["special_functions"] = std::pair<GW_MOD, GW_MOD>(&SpecialFunctionsModule::Load, &SpecialFunctionsModule::Unload);
    m_ModuleMap["graphic_export"] = std::pair<GW_MOD, GW_MOD>(&GraphicExportModule::Load, &GraphicExportModule::Unload);
    m_ModuleMap["polynomials"] = std::pair<GW_MOD, GW_MOD>(&PolynomialsModule::Load, &PolynomialsModule::Unload);
    m_ModuleMap["arnoldi"] = std::pair<GW_MOD, GW_MOD>(&ArnoldiModule::Load, &ArnoldiModule::Unload);
    m_ModuleMap["data_structures"] = std::pair<GW_MOD, GW_MOD>(&DataStructuresModule::Load, &DataStructuresModule::Unload);
    m_ModuleMap["call_scilab"] = std::pair<GW_MOD, GW_MOD>(&CallScilabModule::Load, &CallScilabModule::Unload);
    m_ModuleMap["completion"] = std::pair<GW_MOD, GW_MOD>(&CompletionModule::Load, &CompletionModule::Unload);
    m_ModuleMap["xml"] = std::pair<GW_MOD, GW_MOD>(&XmlModule::Load, &XmlModule::Unload);
    m_ModuleMap["scicos"] = std::pair<GW_MOD, GW_MOD>(&ScicosModule::Load, &ScicosModule::Unload);
    m_ModuleMap["xcos"] = std::pair<GW_MOD, GW_MOD>(&XcosModule::Load, &XcosModule::Unload);
    m_ModuleMap["fftw"] = std::pair<GW_MOD, GW_MOD>(&FFTWModule::Load, &FFTWModule::Unload);
    m_ModuleMap["mpi"] = std::pair<GW_MOD, GW_MOD>(&MPIModule::Load, &MPIModule::Unload);
    m_ModuleMap["external_objects"] = std::pair<GW_MOD, GW_MOD>(&ExternalObjectsModule::Load, &ExternalObjectsModule::Unload);
    m_ModuleMap["external_objects_java"] = std::pair<GW_MOD, GW_MOD>(&ExternalObjectsJavaModule::Load, &ExternalObjectsJavaModule::Unload);
    m_ModuleMap["preferences"] = std::pair<GW_MOD, GW_MOD>(&PreferencesModule::Load, &PreferencesModule::Unload);
    m_ModuleMap["slint"] = std::pair<GW_MOD, GW_MOD>(&SlintModule::Load, &SlintModule::Unload);
    m_ModuleMap["coverage"] = std::pair<GW_MOD, GW_MOD>(&CoverageModule::Load, &SlintModule::Unload);

    if (ConfigVariable::getScilabMode() != SCILAB_NWNI)
    {
        m_ModuleMap["tclsci"] = std::pair<GW_MOD, GW_MOD>(&TclsciModule::Load, &TclsciModule::Unload);
        m_ModuleMap["jvm"] = std::pair<GW_MOD, GW_MOD>(&JvmModule::Load, &JvmModule::Unload);
        m_ModuleMap["ui_data"] = std::pair<GW_MOD, GW_MOD>(&UiDataModule::Load, &UiDataModule::Unload);
    }
#ifdef _MSC_VER
    m_ModuleMap["windows_tools"] = std::pair<GW_MOD, GW_MOD>(&WindowsToolsModule::Load, &WindowsToolsModule::Unload);
#endif
    return true;
}

bool FuncManager::ExecuteFile(const std::string& _stFile)
{
    Parser parser;

    parser.parseFile(_stFile, ConfigVariable::getSCIPath());

    if (parser.getExitStatus() == Parser::Failed)
    {
        std::ostringstream ostr;
        ostr << _("Unable to execute : ") << _stFile << std::endl;
        scilabWrite(ostr.str().c_str());
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
        scilabWrite(ie.GetErrorMessage().c_str());
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

bool FuncManager::ExecuteStartFile(const std::string& _stModule)
{
    //build .start filename
    std::string stPath = ConfigVariable::getSCIPath();
    stPath += MODULE_DIR;
    stPath += _stModule;
    stPath += ETC_DIR;
    stPath += _stModule;
    stPath += START_EXT;

    return ExecuteFile(stPath);
}

bool FuncManager::ExecuteQuitFile(const std::string& _stModule)
{
    //build .quit filename
    std::string stPath = ConfigVariable::getSCIPath();
    stPath += MODULE_DIR;
    stPath += _stModule;
    stPath += ETC_DIR;
    stPath += _stModule;
    stPath += QUIT_EXT;

    return ExecuteFile(stPath);
}
