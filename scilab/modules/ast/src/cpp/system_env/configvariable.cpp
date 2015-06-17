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
#include "configvariable.hxx"
#include "context.hxx"

extern "C"
{
#include "strsubst.h"
#include "os_string.h"
#include "sci_malloc.h"
#include "elem_common.h"
}

/*
** Module List
** \{
*/
std::list<std::wstring> ConfigVariable::m_ModuleList;

void ConfigVariable::setModuleList(std::list<std::wstring>& _pModule_list)
{
    m_ModuleList = _pModule_list;
}

std::list<std::wstring> ConfigVariable::getModuleList()
{
    std::list<std::wstring> moduleList(m_ModuleList);
    return moduleList;
}
/*
** \}
*/

/*
** SCI
** \{
*/
std::wstring ConfigVariable::m_SCIPath;

void ConfigVariable::setSCIPath(const std::wstring& _SCIPath)
{
    m_SCIPath = _SCIPath;
}

std::wstring& ConfigVariable::getSCIPath()
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

std::wstring ConfigVariable::m_SCIHOME;

void ConfigVariable::setSCIHOME(const std::wstring& _SCIHOME)
{
    m_SCIHOME = _SCIHOME;
}

std::wstring& ConfigVariable::getSCIHOME()
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

std::wstring ConfigVariable::m_TMPDIR;

void ConfigVariable::setTMPDIR(const std::wstring& _TMPDIR)
{
    m_TMPDIR = _TMPDIR;
}

std::wstring& ConfigVariable::getTMPDIR()
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

int ConfigVariable::m_iScilabMode = 0;

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
** HOME
** \{
*/

std::wstring ConfigVariable::m_HOME;

void ConfigVariable::setHOME(const std::wstring& _HOME)
{
    m_HOME = _HOME;
}

std::wstring& ConfigVariable::getHOME()
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
        m_wstError          = L"";
        m_iError            = 0;
        m_iErrorLine        = 0;
        m_wstErrorFunction  = L"";
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

std::wstring ConfigVariable::m_wstError;

void ConfigVariable::setLastErrorMessage(const std::wstring& _wstError)
{
    m_wstError = _wstError;
}

std::wstring& ConfigVariable::getLastErrorMessage()
{
    return m_wstError;
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

std::wstring ConfigVariable::m_wstErrorFunction;

void ConfigVariable::setLastErrorFunction(const std::wstring& _wstErrorFunction)
{
    m_wstErrorFunction = _wstErrorFunction;
}

std::wstring& ConfigVariable::getLastErrorFunction()
{
    return m_wstErrorFunction;
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
    if ( m_iPromptMode == 0     ||
            m_iPromptMode == 2  ||
            m_iPromptMode == 3)
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
    if ( m_iPromptMode == 0     ||
            m_iPromptMode == 1  ||
            m_iPromptMode == 2  ||
            m_iPromptMode == 3)
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
    pDL->pwstLibraryName = NULL;
    pDL->hLib = 0;
    return pDL;
}

ConfigVariable::EntryPointStr* ConfigVariable::getNewEntryPointStr()
{
    EntryPointStr* pEP = (EntryPointStr*)MALLOC(sizeof(EntryPointStr));
    pEP->bOK = false;
    pEP->functionPtr = NULL;
    pEP->iLibIndex = -1;
    pEP->pwstEntryPointName = NULL;
    return pEP;
}

void ConfigVariable::setLibraryName(ConfigVariable::DynamicLibraryStr* _pDynamicLibrary, wchar_t* _pwstLibraryName)
{
    if (_pDynamicLibrary)
    {
        if (_pDynamicLibrary->pwstLibraryName)
        {
            FREE(_pDynamicLibrary->pwstLibraryName);
        }
        _pDynamicLibrary->pwstLibraryName = os_wcsdup(_pwstLibraryName);
    }
}

void ConfigVariable::setEntryPointName(ConfigVariable::EntryPointStr* _pEntryPoint, wchar_t* _pwstEntryPointName)
{
    if (_pEntryPoint)
    {
        if (_pEntryPoint->pwstEntryPointName)
        {
            FREE(_pEntryPoint->pwstEntryPointName);
        }
        _pEntryPoint->pwstEntryPointName = os_wcsdup(_pwstEntryPointName);;
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
                FREE(pEP->pwstEntryPointName);
                delete pEP;
                if (m_EntryPointList.size() == 0)
                {
                    break;
                }
                it = m_EntryPointList.begin();
            }
        }
        //remove dynamic library
        FREE(m_DynLibList[_iDynamicLibraryIndex]->pwstLibraryName);
        delete m_DynLibList[_iDynamicLibraryIndex];
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

ConfigVariable::EntryPointStr* ConfigVariable::getEntryPoint(wchar_t* _pwstEntryPointName, int _iDynamicLibraryIndex)
{
    std::list<EntryPointStr*>::const_iterator it;
    for (it = m_EntryPointList.begin() ; it != m_EntryPointList.end() ; it++)
    {
        //by pass iLibIndex check if _iDynamicLibraryIndex == -1
        if (_iDynamicLibraryIndex == -1 || (*it)->iLibIndex == _iDynamicLibraryIndex)
        {
            if (wcscmp((*it)->pwstEntryPointName, _pwstEntryPointName) == 0)
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

int ConfigVariable::getEntryPointPosition(wchar_t* _pwstEntryPointName, int _iDynamicLibraryIndex)
{
    int pos = 0;
    std::list<EntryPointStr*>::const_iterator it;
    for (it = m_EntryPointList.begin(); it != m_EntryPointList.end(); it++, ++pos)
    {
        //by pass iLibIndex check if _iDynamicLibraryIndex == -1
        if (_iDynamicLibraryIndex == -1 || (*it)->iLibIndex == _iDynamicLibraryIndex)
        {
            if (wcscmp((*it)->pwstEntryPointName, _pwstEntryPointName) == 0)
            {
                return pos;
            }
        }
    }
    return -1;
}

std::vector<std::wstring> ConfigVariable::getEntryPointNameList()
{
    std::vector<std::wstring> EntryPointNames;
    std::list<EntryPointStr*>::const_iterator it;
    for (it = m_EntryPointList.begin() ; it != m_EntryPointList.end() ; it++)
    {
        EntryPointNames.push_back((*it)->pwstEntryPointName);
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
std::map<std::wstring, DynLibHandle> ConfigVariable::m_DynModules;

void ConfigVariable::addDynModule(std::wstring _name, DynLibHandle _lib)
{
    m_DynModules[_name] = _lib;
}

void ConfigVariable::removeDynModule(std::wstring _name)
{
    m_DynModules.erase(_name);
}

DynLibHandle ConfigVariable::getDynModule(std::wstring _name)
{
    std::map<std::wstring, DynLibHandle>::iterator it;
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
    std::map<std::wstring, DynLibHandle>::iterator it = m_DynModules.begin();
    std::map<std::wstring, DynLibHandle>::iterator itEnd = m_DynModules.end();
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
std::vector<std::wstring> ConfigVariable::m_Args;
bool ConfigVariable::m_bTimed = false;
bool ConfigVariable::m_bSerialize = false;

void ConfigVariable::setCommandLineArgs(int _iArgs, char** _pstArgs)
{
    m_Args.clear();
    for (int i = 0 ; i < _iArgs ; i++)
    {
        wchar_t * ws = to_wide_string(_pstArgs[i]);
        m_Args.push_back(ws);
        FREE(ws);
    }
}

wchar_t** ConfigVariable::getCommandLineArgs(int* _piCount)
{
    wchar_t** pwstArgs = (wchar_t**)MALLOC(m_Args.size() * sizeof(wchar_t*));
    for (int i = 0 ; i < (int)m_Args.size() ; i++)
    {
        pwstArgs[i] = os_wcsdup(m_Args[i].c_str());
    }

    *_piCount = (int)m_Args.size();
    return pwstArgs;
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
int ConfigVariable::m_iIeee = 0;

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

std::vector<ConfigVariable::WhereEntry> ConfigVariable::m_Where;
std::vector<int> ConfigVariable::m_FirstMacroLine;
void ConfigVariable::where_begin(int _iLineNum, int _iLineLocation, const std::wstring& _wstName)
{
    m_Where.emplace_back(_iLineNum, _iLineLocation, _wstName);
}

void ConfigVariable::where_end()
{
    m_Where.pop_back();
}

const std::vector<ConfigVariable::WhereEntry>& ConfigVariable::getWhere()
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

/*
** \}
*/

/*
** module called with variable by reference
** \{
*/

std::list<std::wstring> ConfigVariable::m_ReferenceModules;

bool ConfigVariable::checkReferenceModule(const std::wstring& _module)
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

void ConfigVariable::addReferenceModule(const std::wstring& _module)
{
    if (checkReferenceModule(_module) == false)
    {
        m_ReferenceModules.push_back(_module);
    }
}

void ConfigVariable::removeReferenceModule(const std::wstring& _module)
{
    if (checkReferenceModule(_module))
    {
        m_ReferenceModules.remove(_module);
    }
}

std::list<std::wstring> ConfigVariable::getReferenceModules()
{
    std::list<std::wstring> l(m_ReferenceModules);
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
