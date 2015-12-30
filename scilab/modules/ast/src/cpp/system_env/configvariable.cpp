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

#include <vector>
#include <list>
#include "context.hxx"
#include "configvariable.hxx"
#include "macrofile.hxx"
#include "threadmanagement.hxx"
#include "execvisitor.hxx"
#include "threadId.hxx"
#include "cell.hxx"
#include "callable.hxx"

extern "C"
{
#include "strsubst.h"
#include "os_string.h"
#include "sci_malloc.h"
#include "elem_common.h"
#include "FileExist.h"
}

/*
** Module List
** \{
*/
std::list<std::string> ConfigVariable::m_ModuleList;

void ConfigVariable::setModuleList(std::list<std::string>& _pModule_list)
{
    m_ModuleList = _pModule_list;
}

std::list<std::string> ConfigVariable::getModuleList()
{
    std::list<std::string> moduleList(m_ModuleList);
    return moduleList;
}
/*
** \}
*/

/*
** SCI
** \{
*/
std::string ConfigVariable::m_SCIPath;

void ConfigVariable::setSCIPath(const std::string& _SCIPath)
{
    m_SCIPath = _SCIPath;
}

std::string& ConfigVariable::getSCIPath()
{
    return m_SCIPath;
}

/*
** \}
*/

/*
** SCIHOME
** \{
*/

std::string ConfigVariable::m_SCIHOME;

void ConfigVariable::setSCIHOME(const std::string& _SCIHOME)
{
    m_SCIHOME = _SCIHOME;
}

std::string& ConfigVariable::getSCIHOME()
{
    return m_SCIHOME;
}
/*
** \}
*/

/*
** TMPDIR
** \{
*/

std::string ConfigVariable::m_TMPDIR;

void ConfigVariable::setTMPDIR(const std::string& _TMPDIR)
{
    m_TMPDIR = _TMPDIR;
}

std::string& ConfigVariable::getTMPDIR()
{
    return m_TMPDIR;
}
/*
** \}
*/

/*
** Force Quit
** \{
*/
bool ConfigVariable::m_bForceQuit = false;

void ConfigVariable::setForceQuit(bool _bForceQuit)
{
    m_bForceQuit = _bForceQuit;
}

bool ConfigVariable::getForceQuit(void)
{
    return m_bForceQuit;
}
/*
** \}
*/

/*
** Exit Status
** \{
*/
int ConfigVariable::m_iExitStatus = 0;

void ConfigVariable::setExitStatus(int _iExitStatus)
{
    m_iExitStatus = _iExitStatus;
}

int ConfigVariable::getExitStatus(void)
{
    return m_iExitStatus;
}
/*
** \}
*/

/*
** Digit precision, ex format function
** \{
*/

int ConfigVariable::m_iFormatSize = 0;

void ConfigVariable::setFormatSize(int _iFormatSize)
{
    m_iFormatSize = _iFormatSize;
}

int ConfigVariable::getFormatSize(void)
{
    return m_iFormatSize;
}

int ConfigVariable::m_iFormatMode = 0;

void ConfigVariable::setFormatMode(int _iFormatMode)
{
    m_iFormatMode = _iFormatMode;
}

int ConfigVariable::getFormatMode(void)
{
    return m_iFormatMode;
}

/*
** \}
*/

/*
** Screen console width
** \{
*/

int ConfigVariable::m_iConsoleWidth = 0;

void ConfigVariable::setConsoleWidth(int _iConsoleWidth)
{
    m_iConsoleWidth = Max(ICONSOLEWIDTH_MIN, _iConsoleWidth);
}

int ConfigVariable::getConsoleWidth(void)
{
    return m_iConsoleWidth;
}
/*
** \}
*/

/*
** Screen console lines
** \{
*/

int ConfigVariable::m_iConsoleLines = 0; //console lines default value

void ConfigVariable::setConsoleLines(int _iConsoleLines)
{
    m_iConsoleLines = Max(ICONSOLELINES_MIN, _iConsoleLines);
}

int ConfigVariable::getConsoleLines(void)
{
    return m_iConsoleLines;
}
/*
** \}
*/

/*
** Scilab mode
** \{
*/

int ConfigVariable::m_iScilabMode = 1; //SCILAB_API = 1  Scilab is launch as an API

void ConfigVariable::setScilabMode(int _iScilabMode)
{
    m_iScilabMode = _iScilabMode;
}

int ConfigVariable::getScilabMode(void)
{
    return m_iScilabMode;
}
/*
** \}
*/

/*
** Warning Mode
** \{
*/
bool ConfigVariable::m_bWarningMode = true;

void ConfigVariable::setWarningMode(bool _bWarningMode)
{
    m_bWarningMode = _bWarningMode;
}

bool ConfigVariable::getWarningMode(void)
{
    return m_bWarningMode;
}
/*
** \}
*/
/*
** \}
*/

/*
** Warning Stop
** \{
*/
bool ConfigVariable::m_bWarningStop = false;

void ConfigVariable::setWarningStop(bool _bWarningStop)
{
    m_bWarningStop = _bWarningStop;
}

bool ConfigVariable::getWarningStop(void)
{
    return m_bWarningStop;
}
/*
** \}
*/


/*
** HOME
** \{
*/

std::string ConfigVariable::m_HOME;

void ConfigVariable::setHOME(const std::string& _HOME)
{
    m_HOME = _HOME;
}

std::string& ConfigVariable::getHOME()
{
    return m_HOME;
}
/*
** \}
*/

/*
** Clear last error information
** \{
*/
bool ConfigVariable::m_bLastErrorCall = false;

void ConfigVariable::setLastErrorCall(void)
{
    m_bLastErrorCall = true;
}

void ConfigVariable::clearLastError(void)
{
    //if (m_bLastErrorCall == false)
    {
        m_stError           = "";
        m_iError            = 0;
        m_iErrorLine        = 0;
        m_stErrorFunction   = "";
    }
    m_bLastErrorCall = false;
}
/*
** \}
*/

/*
** Last Error Message
** \{
*/

std::string ConfigVariable::m_stError;

void ConfigVariable::setLastErrorMessage(const std::string& _stError)
{
    m_stError = _stError;
}

std::string& ConfigVariable::getLastErrorMessage()
{
    return m_stError;
}
/*
** \}
*/

/*
** Last Error ID
** \{
*/
int ConfigVariable::m_iError = 0;
bool ConfigVariable::m_bError = false;

void ConfigVariable::setError()
{
    m_bError = true;
}

bool ConfigVariable::isError()
{
    return m_bError;
}

void ConfigVariable::resetError()
{
    m_bError = false;
}

void ConfigVariable::setLastErrorNumber(int _iError)
{
    m_iError = _iError;
}

int ConfigVariable::getLastErrorNumber(void)
{
    return m_iError;
}
/*
** \}
*/

/*
** Last Error Line
** \{
*/
int ConfigVariable::m_iErrorLine = 0;

void ConfigVariable::setLastErrorLine(int _iErrorLine)
{
    m_iErrorLine = _iErrorLine;
}

int ConfigVariable::getLastErrorLine(void)
{
    return m_iErrorLine;
}
/*
** \}
*/

/*
** Last Error Function
** \{
*/

std::string ConfigVariable::m_stErrorFunction = "";

void ConfigVariable::setLastErrorFunction(const std::string& _stErrorFunction)
{
    m_stErrorFunction = _stErrorFunction;
}

std::string& ConfigVariable::getLastErrorFunction()
{
    return m_stErrorFunction;
}

/*
** \}
*/

/*
** Prompt Mode
** \{
*/

int ConfigVariable::m_iPromptMode = 0;
int ConfigVariable::m_iSilentError = 0;
bool ConfigVariable::m_bVerbose = true;

void ConfigVariable::setPromptMode(int _iPromptMode)
{
    m_iPromptMode = _iPromptMode;
    if (m_iPromptMode == 5)
    {
        m_iPromptMode = 1;
    }

    if (m_iPromptMode == 6)
    {
        m_iPromptMode = 7;
    }
}

int ConfigVariable::getPromptMode(void)
{
    return m_iPromptMode;
}

bool ConfigVariable::isEmptyLineShow(void)
{
    if ( m_iPromptMode == 0    ||
            m_iPromptMode == 2 ||
            m_iPromptMode == 3 ||
            m_iPromptMode == 7)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool ConfigVariable::isPromptShow(void)
{
    if ( m_iPromptMode == 0    ||
            m_iPromptMode == 1 ||
            m_iPromptMode == 2 ||
            m_iPromptMode == 3 ||
            m_iPromptMode == 7)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void ConfigVariable::setSilentError(int _iSilentError)
{
    m_iSilentError = _iSilentError;
}

int ConfigVariable::getSilentError(void)
{
    return m_iSilentError;
}

void ConfigVariable::setVerbose(bool _bVerbose)
{
    m_bVerbose = _bVerbose;
}

bool ConfigVariable::getVerbose(void)
{
    return m_bVerbose;
}

/*
** ThreadList
** \{
*/

std::list<types::ThreadId *> ConfigVariable::m_threadList;

types::ThreadId* ConfigVariable::getLastPausedThread()
{
    std::list<types::ThreadId *>::reverse_iterator it;
    for (it = m_threadList.rbegin() ; it != m_threadList.rend() ; it++)
    {
        if ((*it)->getStatus() == types::ThreadId::Paused)
        {
            return *it;
        }
    }
    return NULL;
}

types::ThreadId* ConfigVariable::getLastRunningThread()
{
    std::list<types::ThreadId *>::reverse_iterator it;
    for (it = m_threadList.rbegin() ; it != m_threadList.rend() ; it++)
    {
        if ((*it)->getStatus() == types::ThreadId::Running)
        {
            return *it;
        }
    }
    return NULL;
}

types::ThreadId* ConfigVariable::getLastThread()
{
    return m_threadList.back();
}

types::Cell* ConfigVariable::getAllThreads(void)
{
    int iSize = (int) ConfigVariable::m_threadList.size();

    if (iSize == 0)
    {
        return new types::Cell();
    }

    int i = 0;
    types::Cell *pcResult = new types::Cell(iSize, 1);
    std::list<types::ThreadId *>::iterator it;

    for (auto thread : ConfigVariable::m_threadList)
    {
        pcResult->set(i++, *it);
    }

    return pcResult;
}


void ConfigVariable::addThread(types::ThreadId* _thread)
{
    _thread->IncreaseRef();
    m_threadList.push_back(_thread);
}


types::ThreadId* ConfigVariable::getThread(__threadKey _key)
{
    std::list<types::ThreadId *>::const_iterator it;

    for (it = ConfigVariable::m_threadList.begin() ; it != ConfigVariable::m_threadList.end() ; ++it)
    {
        if ((*it)->getKey() == _key)
        {
            return *it;
        }
    }
    return NULL;
}


void ConfigVariable::deleteThread(__threadKey _key)
{
    std::list<types::ThreadId *>::iterator it;
    for (it = ConfigVariable::m_threadList.begin() ; it != ConfigVariable::m_threadList.end() ; ++it)
    {
        if ((*it)->getKey() == _key)
        {
            (*it)->DecreaseRef();
            if ((*it)->isDeletable())
            {
                (*it)->killMe();
                (*it) = NULL;
                m_threadList.erase(it);
                return;
            }
        }
    }
}

/*
** \}
*/

/*
** Prompt Mode
** \{
*/

int ConfigVariable::m_iPauseLevel = 0;
std::list<int> ConfigVariable::m_listScope;

void ConfigVariable::IncreasePauseLevel()
{
    m_iPauseLevel++;
    m_listScope.push_back(symbol::Context::getInstance()->getScopeLevel());
}

void ConfigVariable::DecreasePauseLevel()
{
    m_iPauseLevel--;
    m_listScope.pop_back();
}

int ConfigVariable::getActivePauseLevel()
{
    return m_listScope.back();
}

int ConfigVariable::getPauseLevel()
{
    return m_iPauseLevel;
}

/*
** \}
*/

/*
** Dynamic Link
** \{
*/

std::vector<ConfigVariable::DynamicLibraryStr*> ConfigVariable::m_DynLibList;
std::list<ConfigVariable::EntryPointStr*> ConfigVariable::m_EntryPointList;


ConfigVariable::DynamicLibraryStr* ConfigVariable::getNewDynamicLibraryStr()
{
    DynamicLibraryStr* pDL = (DynamicLibraryStr*)MALLOC(sizeof(DynamicLibraryStr));
    pDL->pstLibraryName = NULL;
    pDL->hLib = 0;
    return pDL;
}

ConfigVariable::EntryPointStr* ConfigVariable::getNewEntryPointStr()
{
    EntryPointStr* pEP = (EntryPointStr*)MALLOC(sizeof(EntryPointStr));
    pEP->bOK = false;
    pEP->functionPtr = NULL;
    pEP->iLibIndex = -1;
    pEP->pstEntryPointName = NULL;
    return pEP;
}

void ConfigVariable::setLibraryName(ConfigVariable::DynamicLibraryStr* _pDynamicLibrary, const char* _pstLibraryName)
{
    if (_pDynamicLibrary)
    {
        if (_pDynamicLibrary->pstLibraryName)
        {
            FREE(_pDynamicLibrary->pstLibraryName);
        }
        _pDynamicLibrary->pstLibraryName = os_strdup(_pstLibraryName);
    }
}

void ConfigVariable::setEntryPointName(ConfigVariable::EntryPointStr* _pEntryPoint, const char* _pstEntryPointName)
{
    if (_pEntryPoint)
    {
        if (_pEntryPoint->pstEntryPointName)
        {
            FREE(_pEntryPoint->pstEntryPointName);
        }
        _pEntryPoint->pstEntryPointName = os_strdup(_pstEntryPointName);;
    }
}

/* Dynamic libraries functions */
int ConfigVariable::addDynamicLibrary(ConfigVariable::DynamicLibraryStr* _pDynamicLibrary)
{
    for (int i = 0 ; i < (int)m_DynLibList.size() ; i++)
    {
        if (m_DynLibList[i] == NULL)
        {
            m_DynLibList[i] = _pDynamicLibrary;
            return i;
        }
    }

    m_DynLibList.push_back(_pDynamicLibrary);
    return (int)m_DynLibList.size() - 1;
}

void ConfigVariable::removeDynamicLibrary(int _iDynamicLibraryIndex)
{
    if (_iDynamicLibraryIndex < (int)m_DynLibList.size())
    {
        std::list<EntryPointStr*>::const_iterator it;
        for (it = m_EntryPointList.begin() ; it != m_EntryPointList.end() ; it++)
        {
            //clear all entry points linked to removed dynamic library
            if ((*it)->iLibIndex == _iDynamicLibraryIndex)
            {
                EntryPointStr* pEP = *it;
                m_EntryPointList.remove(*it);
                FREE(pEP->pstEntryPointName);
                FREE(pEP);
                if (m_EntryPointList.size() == 0)
                {
                    break;
                }
                it = m_EntryPointList.begin();
            }
        }
        //remove dynamic library
        FREE(m_DynLibList[_iDynamicLibraryIndex]->pstLibraryName);
        FREE(m_DynLibList[_iDynamicLibraryIndex]);
        m_DynLibList[_iDynamicLibraryIndex] = NULL;
    }

    //clean dynamic library vector
    while (m_DynLibList.size() != 0 && m_DynLibList.back() == NULL)
    {
        m_DynLibList.pop_back();
    }
}

ConfigVariable::DynamicLibraryStr* ConfigVariable::getDynamicLibrary(int _iDynamicLibraryIndex)
{
    if (_iDynamicLibraryIndex < (int)m_DynLibList.size())
    {
        return m_DynLibList[_iDynamicLibraryIndex];
    }
    return NULL;
}

bool ConfigVariable::isDynamicLibrary(int _iDynamicLibraryIndex)
{
    if (_iDynamicLibraryIndex < (int)m_DynLibList.size())
    {
        if (m_DynLibList[_iDynamicLibraryIndex] != NULL)
        {
            return true;
        }
    }
    return false;
}

void ConfigVariable::addEntryPoint(ConfigVariable::EntryPointStr* _pEP)
{
    if (_pEP != NULL)
    {
        m_EntryPointList.push_back(_pEP);
    }
}

ConfigVariable::EntryPointStr* ConfigVariable::getEntryPoint(const char* _pstEntryPointName, int _iDynamicLibraryIndex)
{
    std::list<EntryPointStr*>::const_iterator it;
    for (it = m_EntryPointList.begin() ; it != m_EntryPointList.end() ; it++)
    {
        //by pass iLibIndex check if _iDynamicLibraryIndex == -1
        if (_iDynamicLibraryIndex == -1 || (*it)->iLibIndex == _iDynamicLibraryIndex)
        {
            if (strcmp((*it)->pstEntryPointName, _pstEntryPointName) == 0)
            {
                return *it;
            }
        }
    }
    return NULL;
}

dynlib_ptr ConfigVariable::getEntryPointFromPosition(int position)
{
    std::list<EntryPointStr*>::const_iterator it;
    int pos = 0;
    for (it = m_EntryPointList.begin(); it != m_EntryPointList.end(); it++, ++pos)
    {
        if (pos == position)
        {
            return (*it)->functionPtr;
        }
    }
    return NULL;
}

int ConfigVariable::getEntryPointPosition(const char* _pstEntryPointName, int _iDynamicLibraryIndex)
{
    int pos = 0;
    std::list<EntryPointStr*>::const_iterator it;
    for (it = m_EntryPointList.begin(); it != m_EntryPointList.end(); it++, ++pos)
    {
        //by pass iLibIndex check if _iDynamicLibraryIndex == -1
        if (_iDynamicLibraryIndex == -1 || (*it)->iLibIndex == _iDynamicLibraryIndex)
        {
            if (strcmp((*it)->pstEntryPointName, _pstEntryPointName) == 0)
            {
                return pos;
            }
        }
    }
    return -1;
}

std::vector<std::string> ConfigVariable::getEntryPointNameList()
{
    std::vector<std::string> EntryPointNames;
    std::list<EntryPointStr*>::const_iterator it;
    for (it = m_EntryPointList.begin() ; it != m_EntryPointList.end() ; it++)
    {
        EntryPointNames.push_back((*it)->pstEntryPointName);
    }
    return EntryPointNames;
}

std::vector<ConfigVariable::DynamicLibraryStr*>* ConfigVariable::getDynamicLibraryList()
{
    return &m_DynLibList;
}

std::list<ConfigVariable::EntryPointStr*>* ConfigVariable::getEntryPointList()
{
    return &m_EntryPointList;
}

//dynamic modules
std::map<std::string, DynLibHandle> ConfigVariable::m_DynModules;

void ConfigVariable::addDynModule(const std::string& _name, DynLibHandle _lib)
{
    m_DynModules[_name] = _lib;
}

void ConfigVariable::removeDynModule(const std::string& _name)
{
    m_DynModules.erase(_name);
}

DynLibHandle ConfigVariable::getDynModule(const std::string& _name)
{
    std::map<std::string, DynLibHandle>::iterator it;
    it = m_DynModules.find(_name);
    if (it != m_DynModules.end())
    {
        return it->second;
    }

    return 0;
}

int ConfigVariable::getDynModuleCount()
{
    return (int)m_DynModules.size();
}

DynLibHandle* ConfigVariable::getAllDynModule()
{
    DynLibHandle* moduleList = new DynLibHandle[m_DynModules.size()];
    std::map<std::string, DynLibHandle>::iterator it = m_DynModules.begin();
    std::map<std::string, DynLibHandle>::iterator itEnd = m_DynModules.end();
    for (int i = 0; it != itEnd ; ++it, ++i)
    {
        moduleList[i] = it->second;
    }

    return moduleList;
}

void ConfigVariable::cleanDynModule()
{
    m_DynModules.clear();
}

// Command Line Arguments
std::vector<std::string> ConfigVariable::m_Args;
bool ConfigVariable::m_bTimed = false;
bool ConfigVariable::m_bSerialize = false;

void ConfigVariable::setCommandLineArgs(int _iArgs, char** _pstArgs)
{
    m_Args.clear();
    for (int i = 0 ; i < _iArgs ; i++)
    {
        m_Args.push_back(_pstArgs[i]);
    }
}

char** ConfigVariable::getCommandLineArgs(int* _piCount)
{
    char** pstArgs = NULL;
    *_piCount = (int)m_Args.size();
    if (*_piCount != 0)
    {
        pstArgs = (char**)MALLOC(*_piCount * sizeof(char*));
        for (int i = 0; i < *_piCount; i++)
        {
            pstArgs[i] = os_strdup(m_Args[i].c_str());
        }
    }
    return pstArgs;
}

bool ConfigVariable::getTimed()
{
    return m_bTimed;
}

void ConfigVariable::setTimed(bool _bTimed)
{
    m_bTimed = _bTimed;
}

bool ConfigVariable::getSerialize()
{
    return m_bSerialize;
}

void ConfigVariable::setSerialize(bool _bSerialize)
{
    m_bSerialize = _bSerialize;
}

/*
** \}
*/

///*
//** Input Method
//** \{
//*/
//
//SCILAB_INPUT_METHOD ConfigVariable::m_pInputMethod = NULL;
//
//void ConfigVariable::setInputMethod(SCILAB_INPUT_METHOD _pInputMethod)
//{
//    m_pInputMethod = _pInputMethod;
//}
//
//SCILAB_INPUT_METHOD ConfigVariable::getInputMethod(void)
//{
//    return m_pInputMethod;
//}
//
///*
//** \}
//*/
//
///*
//** Output Method
//** \{
//*/
//
//SCILAB_OUTPUT_METHOD ConfigVariable::m_pOutputMethod = NULL;
//
//void ConfigVariable::setOutputMethod(SCILAB_OUTPUT_METHOD _pOutputMethod)
//{
//    m_pOutputMethod = _pOutputMethod;
//}
//
//SCILAB_OUTPUT_METHOD ConfigVariable::getOutputMethod(void)
//{
//    return m_pOutputMethod;
//}
//
///*
//** \}
//*/

/*
** schur function
** \{
*/

types::Callable* ConfigVariable::m_schurFunction = NULL;

void ConfigVariable::setSchurFunction(types::Callable* _schurFunction)
{
    m_schurFunction = _schurFunction;
}

types::Callable* ConfigVariable::getSchurFunction()
{
    return m_schurFunction;
}

/*
** \}
*/

/*
** grand (module randlib)
** \{
*/

int ConfigVariable::m_currentBaseGen = 0;
int ConfigVariable::m_currentClcg4   = 0;

void ConfigVariable::setCurrentBaseGen(int _gen)
{
    m_currentBaseGen = _gen;
}

int ConfigVariable::getCurrentBaseGen()
{
    return m_currentBaseGen;
}

void ConfigVariable::setCurrentClcg4(int _clcg4)
{
    m_currentClcg4 = _clcg4;
}

int ConfigVariable::getCurrentClcg4()
{
    return m_currentClcg4;
}
/*
** \}
*/

/*
** Start finish flag
** \{
*/
bool ConfigVariable::m_bStartProcessing = false;
bool ConfigVariable::m_bEndProcessing = false;

void ConfigVariable::setStartProcessing(bool _bStartProcessing)
{
    m_bStartProcessing = _bStartProcessing;
}

bool ConfigVariable::getStartProcessing()
{
    return m_bStartProcessing;
}

void ConfigVariable::setEndProcessing(bool _bEndProcessing)
{
    m_bEndProcessing = _bEndProcessing;
}

bool ConfigVariable::getEndProcessing()
{
    return m_bEndProcessing;
}
/*
** \}
*/

/*
** ieee
** \{
*/
int ConfigVariable::m_iIeee = 2;

void ConfigVariable::setIeee(int _iIeee)
{
    m_iIeee = _iIeee;
}

int ConfigVariable::getIeee()
{
    return m_iIeee;
}
/*
** \}
*/

/*
** simp Mode
** \{
*/
int ConfigVariable::m_iSimpMode = 1;

void ConfigVariable::setSimpMode(int _iSimpMode)
{
    m_iSimpMode = _iSimpMode;
}

int ConfigVariable::getSimpMode()
{
    return m_iSimpMode;
}
/*
** \}
*/

/*
** funcprot Mode
** \{
*/
int ConfigVariable::m_iFuncprot = 1;

void ConfigVariable::setFuncprot(int _iFuncprot)
{
    m_iFuncprot = _iFuncprot;
}

int ConfigVariable::getFuncprot()
{
    return m_iFuncprot;
}
/*
** \}
*/

/*
** where
** \{
*/

ConfigVariable::WhereVector ConfigVariable::m_Where;
ConfigVariable::WhereVector ConfigVariable::m_WhereError;
std::vector<int> ConfigVariable::m_FirstMacroLine;
void ConfigVariable::where_begin(int _iLineNum, int _iLineLocation, types::Callable* _pCall)
{
    std::string wstrFileName("");
    types::Callable* pCall = _pCall;
    if (pCall->isMacroFile())
    {
        types::Macro* pM = pCall->getAs<types::MacroFile>()->getMacro();
        wstrFileName = pM->getFileName();
        pCall = pM;
    }
    else if (pCall->isMacro())
    {
        types::Macro* pM = pCall->getAs<types::Macro>();
        wstrFileName = pM->getFileName();
    }

    m_Where.emplace_back(_iLineNum, _iLineLocation, pCall->getName(), pCall->getFirstLine(), wstrFileName);
}

void ConfigVariable::where_end()
{
    m_Where.pop_back();
}

const ConfigVariable::WhereVector& ConfigVariable::getWhere()
{
    return m_Where;
}

void ConfigVariable::macroFirstLine_begin(int _iLine)
{
    m_FirstMacroLine.push_back(_iLine);
}

void ConfigVariable::macroFirstLine_end()
{
    m_FirstMacroLine.pop_back();
}

int ConfigVariable::getMacroFirstLines()
{
    if (m_FirstMacroLine.empty())
    {
        return 1;
    }

    return m_FirstMacroLine.back();
}
void ConfigVariable::setFileNameToLastWhere(const std::string& _fileName)
{
    m_Where.back().m_file_name = _fileName;
}

void ConfigVariable::whereErrorToString(std::ostringstream &ostr)
{
    int iLenName = 1;
    bool isExecstr = false;
    bool isExecfile = false;

    // get max length of functions name and check if exec or execstr have been called.
    for (auto & where : m_WhereError)
    {
        if (isExecstr == false && where.m_name == "execstr")
        {
            isExecstr = true;
            continue;
        }
        else if (isExecfile == false && where.m_name == "exec")
        {
            isExecfile = true;
            continue;
        }

        iLenName = std::max((int)where.m_name.length(), iLenName);

        // in case of bin file, the file path and line is displayed only if the associated .sci file exists
        if (where.m_file_name != "" && where.m_file_name.find(".bin") != std::string::npos)
        {
            std::size_t pos = where.m_file_name.find_last_of(".");
            where.m_file_name.replace(pos, pos + 4, ".sci");
            if (FileExist(where.m_file_name.c_str()) == false)
            {
                where.m_file_name = "";
            }
        }
    }

    // add margin
    iLenName++;

    // initialize localized strings
    std::string strBuiltin(_("in builtin "));
    std::string strAtLine(_("at line % 5d of function "));
    std::string strExecStr(_("at line % 5d of executed string "));
    std::string strExecFile(_("at line % 5d of executed file "));

    // compute max size between "at line xxx of function" and "in builtin "
    // +1 : line number is pad to 5. length of "% 5d" + 1 == 5
    int iMaxLen = std::max(strAtLine.length() + 1, strBuiltin.length());
    if (isExecstr)
    {
        iMaxLen = std::max(((int)strExecStr.length()) + 1, iMaxLen);
    }

    if (isExecstr)
    {
        iMaxLen = std::max(((int)strExecFile.length()) + 1, iMaxLen);
    }

    // print call stack
    ostr << std::left;
    ostr.fill(L' ');
    for (auto & where : m_WhereError)
    {
        ostr.width(iMaxLen);
        if (where.m_line == 0)
        {
            ostr << strBuiltin;
        }
        else
        {
            char csTmp[bsiz];
            if (where.m_name == "execstr")
            {
                isExecstr = true;
                os_sprintf(csTmp, strExecStr.c_str(), where.m_line);
                ostr << csTmp << std::endl;
                continue;
            }
            else if (where.m_name == "exec")
            {
                os_sprintf(csTmp, strExecFile.c_str(), where.m_line);
                ostr << csTmp << where.m_file_name << std::endl;
                continue;
            }
            else
            {
                os_sprintf(csTmp, strAtLine.c_str(), where.m_line);
                ostr << csTmp;
            }
        }

        ostr.width(iLenName);
        ostr << where.m_name;

        if (where.m_file_name != "")
        {
            // -1 because the first line of a function dec is : "function myfunc()"
            ostr << "( " << where.m_file_name << " " << _("line") << " " << where.m_macro_first_line + where.m_line - 1 << " )";
        }

        ostr << std::endl;
    }

    ostr << std::endl;
}

void ConfigVariable::fillWhereError(int _iErrorLine)
{
    if (m_WhereError.empty())
    {
        int iTmp = 0;
        if (_iErrorLine != 0)
        {
            // +1 because the first line of the funtionDec "function func()" is the line 1.
            iTmp = _iErrorLine - getMacroFirstLines() + 1;
        }

        m_WhereError.reserve(m_Where.size());
        for (auto where = m_Where.rbegin(); where != m_Where.rend(); ++where)
        {
            m_WhereError.emplace_back(iTmp, (*where).m_absolute_line, (*where).m_name, (*where).m_macro_first_line, (*where).m_file_name);
            iTmp = (*where).m_line;
        }
    }
}

void ConfigVariable::resetWhereError()
{
    m_WhereError.clear();
}

/*
** \}
*/

/*
** module called with variable by reference
** \{
*/

std::list<std::string> ConfigVariable::m_ReferenceModules;

bool ConfigVariable::checkReferenceModule(const std::string& _module)
{
    for (auto ref : m_ReferenceModules)
    {
        if (ref == _module)
        {
            return true;
        }
    }

    return false;
}

void ConfigVariable::addReferenceModule(const std::string& _module)
{
    if (checkReferenceModule(_module) == false)
    {
        m_ReferenceModules.push_back(_module);
    }
}

void ConfigVariable::removeReferenceModule(const std::string& _module)
{
    if (checkReferenceModule(_module))
    {
        m_ReferenceModules.remove(_module);
    }
}

std::list<std::string> ConfigVariable::getReferenceModules()
{
    std::list<std::string> l(m_ReferenceModules);
    return l;
}

/*
** \}
*/

/*
** analyzer options
** \{
*/

int ConfigVariable::m_analyzerOptions = 0;
void ConfigVariable::setAnalyzerOptions(int _val)
{
    m_analyzerOptions = _val;
}

int ConfigVariable::getAnalyzerOptions(void)
{
    return m_analyzerOptions;
}

/*
** \}
*/

/*
** divide by zero
** \{
*/

bool ConfigVariable::m_dividebyzero = false;
void ConfigVariable::setDivideByZero(bool _dividebyzero)
{
    m_dividebyzero = _dividebyzero;
}

bool ConfigVariable::isDivideByZero(void)
{
    return m_dividebyzero;
}
/*
** \}
*/

//mex info
std::string ConfigVariable::mexFunctionName;
void ConfigVariable::setMexFunctionName(const std::string& name)
{
    mexFunctionName = name;
}

std::string& ConfigVariable::getMexFunctionName()
{
    return mexFunctionName;
}

/*
** \}
*/
// executed file with exec
int ConfigVariable::m_iFileID = 0;
void ConfigVariable::setExecutedFileID(int _iFileID)
{
    m_iFileID = _iFileID;
}

int ConfigVariable::getExecutedFileID()
{
    return m_iFileID;
}

/*
** string read from console by scilabRead
** \{
*/
char* ConfigVariable::m_pcConsoleReadStr = NULL;
void ConfigVariable::setConsoleReadStr(char* _pcConsoleReadStr)
{
    m_pcConsoleReadStr = _pcConsoleReadStr;
}

char* ConfigVariable::getConsoleReadStr()
{
    ThreadManagement::LockScilabRead();
    char* tmp = m_pcConsoleReadStr;
    m_pcConsoleReadStr = NULL;
    ThreadManagement::UnlockScilabRead();
    return tmp;
}
/*
** \}
*/

/*
** Tell to the console thread if the scilabRead return
** is a scilab command or not.
** \{
*/
int ConfigVariable::m_isScilabCommand = 1;
void ConfigVariable::setScilabCommand(int _isciCmd)
{
    m_isScilabCommand = _isciCmd;
}

int ConfigVariable::isScilabCommand()
{
    return m_isScilabCommand;
}
/*
** \}
*/

//debugger information
bool ConfigVariable::m_bEnabledebug = false;
std::unique_ptr<ast::ConstVisitor> ConfigVariable::m_defaultvisitor(nullptr);

bool ConfigVariable::getEnableDebug()
{
    return m_bEnabledebug;
}

void ConfigVariable::setEnableDebug(bool _enable)
{
    m_bEnabledebug = _enable;
}

void ConfigVariable::setDefaultVisitor(ast::ConstVisitor* _default)
{
    m_defaultvisitor.reset(_default);
}

ast::ConstVisitor* ConfigVariable::getDefaultVisitor()
{
    if (m_defaultvisitor.get() == nullptr)
    {
        m_defaultvisitor.reset(new ast::ExecVisitor());
    }
    return m_defaultvisitor->clone();
}

bool ConfigVariable::executionbreak = false;

bool ConfigVariable::isExecutionBreak()
{
    return executionbreak;
}

void ConfigVariable::setExecutionBreak()
{
    executionbreak = true;
}

void ConfigVariable::resetExecutionBreak()
{
    executionbreak = false;
}


#ifdef _DEBUG
int ConfigVariable::recursionLimit = 25;
#else
int ConfigVariable::recursionLimit = 1000;
#endif
int ConfigVariable::recursionLevel = 0;

int ConfigVariable::getRecursionLimit()
{
    return recursionLimit;
}

int ConfigVariable::setRecursionLimit(int val)
{
    int old = recursionLimit;
    recursionLimit = std::max(10, val);
    return old;
}

int ConfigVariable::getRecursionLevel()
{
    return recursionLevel;
}

bool ConfigVariable::increaseRecursion()
{
    if (recursionLevel < recursionLimit)
    {
        ++recursionLevel;
        return true;
    }

    return false;
}

void ConfigVariable::decreaseRecursion()
{
    //recursionLevel = std::max(--recursionLevel, 0);
    --recursionLevel;
}

void ConfigVariable::resetRecursionLevel()
{
    recursionLevel = 0;
}
