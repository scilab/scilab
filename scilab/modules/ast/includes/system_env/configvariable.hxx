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

#ifndef __CONFIGVARIABLE_HXX__
#define __CONFIGVARIABLE_HXX__

//disable warnings about exports STL items
#ifdef _MSC_VER
#pragma warning (disable : 4251)
#endif

#include <vector>
#include <list>
#include <map>
#include <string>
#include "callable.hxx"
#include "threadId.hxx"
#include "cell.hxx"

extern "C"
{
#include "Thread_Wrapper.h"
#include "dynamiclibrary.h"
#include "dynlib_ast.h"
}

// Minimal values for iConsoleLines & iConsoleWidth
#define ICONSOLELINES_MIN 0
#define ICONSOLEWIDTH_MIN 10

class EXTERN_AST ConfigVariable
{
    //module list
private :
    static std::list<std::wstring> m_ModuleList;

public :
    static void setModuleList(std::list<std::wstring>& _module_list);
    static std::list<std::wstring> getModuleList();


    //SCI
private :
    static std::wstring m_SCIPath;

public :
    static void setSCIPath(const std::wstring& _SCIPath);
    static std::wstring& getSCIPath();

    //SCIHOME
private :
    static std::wstring m_SCIHOME;

public :
    static void setSCIHOME(const std::wstring& _m_SCIHOME);
    static std::wstring& getSCIHOME();

    //TMPDIR
private :
    static std::wstring m_TMPDIR;

public :
    static void setTMPDIR(const std::wstring& _TMPDIR);
    static std::wstring& getTMPDIR();

    // Force Quit
private :
    static bool m_bForceQuit;

public :
    static void setForceQuit(bool _bForceQuit);
    static bool getForceQuit(void);

    // Exit Status
private :
    static int m_iExitStatus;

public :
    static void setExitStatus(int _iExitStatus);
    static int getExitStatus(void);

    // Digit precision, ex format function
private :
    static int m_iFormatSize;

public :
    static void setFormatSize(int _iFormatSize);
    static int getFormatSize(void);

    // printf format ( 0 -> "e", 1 -> "v")
private :
    static int m_iFormatMode;

public :
    static void setFormatMode(int _iFormatMode);
    static int getFormatMode(void);


    // Screen console width
private :
    static int m_iConsoleWidth;

public :
    static void setConsoleWidth(int _iConsoleWidth);
    static int getConsoleWidth(void);

    // Screen console lines
private :
    static int m_iConsoleLines;

public :
    static void setConsoleLines(int _iConsoleLines);
    static int getConsoleLines(void);

    // Scilab mode
private :
    static int m_iScilabMode;

public :
    static void setScilabMode(int _iScilabMode);
    static int getScilabMode(void);

    //Warning mode
private :
    static bool m_bWarningMode;

public :
    static void setWarningMode(bool _bWarningMode);
    static bool getWarningMode(void);

    //HOME
private :
    static std::wstring m_HOME;

public :
    static void setHOME(const std::wstring& _m_HOME);
    static std::wstring& getHOME();

    //Clear last error information
public :
    static void setLastErrorClear(void);

    //Clear last error information
private :
    static bool m_bLastErrorCall;
public :
    static void setLastErrorCall(void);
    static void clearLastError(void);

    //Last Error Message
private :
    static std::wstring m_wstError;

public :
    static void setLastErrorMessage(const std::wstring& _wstError);
    static std::wstring& getLastErrorMessage();

    //Last Error ID
private :
    static int m_iError;
    static bool m_bError;

public :
    static void setLastErrorNumber(int _iError);
    static int getLastErrorNumber();
    static void setError();
    static bool isError();
    static void resetError();

    //Last Error Line
private :
    static int m_iErrorLine;

public :
    static void setLastErrorLine(int _iLine);
    static int getLastErrorLine();

    //Last Error Function
private :
    static std::wstring m_wstErrorFunction;

public :
    static void setLastErrorFunction(const std::wstring& _wstFunction);
    static std::wstring& getLastErrorFunction();

    //Prompt Mode and Silent error
public :
    /*
    Prompt mode
    normal = 0,
    silent = -1,
    prompt = 2,
    exec = 1,
    exec3 = 3,
    step = 4,
    step7 = 7
    */

    /*
       show = 0
       silent = 1
    */
private :
    static int m_iPromptMode;
    static int m_iSilentError;

    //set before function call to know status of e.isVerbose in functions
    static bool m_bVerbose;

public :
    static void setPromptMode(int _iPromptMode);
    static int getPromptMode(void);
    static bool isPromptShow(void);
    static bool isEmptyLineShow(void);

    static void setSilentError(int _iSilentError);
    static int getSilentError(void);
    static void setVerbose(bool _bVerbose);
    static bool getVerbose(void);

    //Thread List

private :
    static std::list<types::ThreadId*> m_threadList;
public :

    static types::ThreadId* getThread(__threadKey _key);
    static types::ThreadId* getLastRunningThread();
    static types::ThreadId* getLastPausedThread();
    static types::ThreadId* getLastThread();

    static types::Cell* getAllThreads(void);
    static void addThread(types::ThreadId* _thread);
    static void deleteThread(__threadKey _key);

    // Pause level
private :
    static int m_iPauseLevel;
    static std::list<int> m_listScope;

public :
    static void IncreasePauseLevel();
    static void DecreasePauseLevel();
    static int getPauseLevel();
    static int getActivePauseLevel();
    // Dynamic Libraries

    typedef struct
    {
        wchar_t* pwstLibraryName;      /** name of dynamic library **/
        DynLibHandle hLib;              /** handle of the library **/
    } DynamicLibraryStr;

    typedef void(*dynlib_ptr)();

    typedef struct
    {
        wchar_t* pwstEntryPointName;    /** name of interface **/
        int iLibIndex;                  /** name of interface **/
        dynlib_ptr functionPtr;         /** entrypoint for the interface **/
        bool bOK;                       /** flag set to TRUE if entrypoint can be used **/
    } EntryPointStr;

private :
    static std::list<EntryPointStr*> m_EntryPointList;
    static std::vector<DynamicLibraryStr*> m_DynLibList;
public :

    /* tools fucntions */
    static void setLibraryName(DynamicLibraryStr* _pDynamicLibrary, wchar_t* _pwstLibraryName);
    static void setEntryPointName(EntryPointStr* _pEntryPoint, wchar_t* _pwstEntryPointName);

    /* "Constructors" */
    static DynamicLibraryStr* getNewDynamicLibraryStr();
    static EntryPointStr* getNewEntryPointStr();

    /* Dynamic libraries functions */
    static std::vector<DynamicLibraryStr*>* getDynamicLibraryList();
    static int addDynamicLibrary(DynamicLibraryStr* _pDynamicLibrary);
    static void removeDynamicLibrary(int _iDynamicLibraryIndex);
    static DynamicLibraryStr* getDynamicLibrary(int _iDynamicLibraryIndex);
    static bool isDynamicLibrary(int _iDynamicLibraryIndex);

    /* Entry point functions */
    static std::list<EntryPointStr*>* getEntryPointList();
    static void addEntryPoint(EntryPointStr* _pEP);
    static void removeEntryPoint(int _iEntryPointIndex);
    static EntryPointStr* getEntryPoint(wchar_t* _pwstEntryPointName, int _iDynamicLibraryIndex = -1);
    static int getEntryPointPosition(wchar_t* _pwstEntryPointName, int _iDynamicLibraryIndex = -1);
    static dynlib_ptr getEntryPointFromPosition(int position);
    static std::vector<std::wstring> getEntryPointNameList();

    //dynamic modules
private :
    static std::map<std::wstring, DynLibHandle> m_DynModules;
public :
    static void addDynModule(std::wstring _name, DynLibHandle _lib);
    static void removeDynModule(std::wstring _name);
    static DynLibHandle getDynModule(std::wstring _name);
    static int getDynModuleCount();
    static DynLibHandle* getAllDynModule();
    static void cleanDynModule();

    // Command Line Arguments
private :
    static std::vector<std::wstring> m_Args;
    static bool m_bTimed;
    static bool m_bSerialize;
public :
    static void setCommandLineArgs(int _iArgs, char** _pstArgs);
    static wchar_t** getCommandLineArgs(int* _piCount);
    static bool getTimed();
    static void setTimed(bool _bTimed);
    static bool getSerialize();
    static void setSerialize(bool _bSerialize);

    //schur function
private :
    static types::Callable* m_schurFunction;

public :
    static void setSchurFunction(types::Callable* _schurFunction);
    static types::Callable* getSchurFunction();

    //grand (module randlib)
private :
    static int m_currentBaseGen;
    static int m_currentClcg4;

public :
    static void setCurrentBaseGen(int _currentBaseGen);
    static int getCurrentBaseGen();

    static void setCurrentClcg4(int _currentClcg4);
    static int getCurrentClcg4();

    //Start and end finish flag
private :
    static bool m_bStartProcessing;
    static bool m_bEndProcessing;

public :
    static void setStartProcessing(bool _bStartProcessing);
    static bool getStartProcessing();
    static void setEndProcessing(bool _bEndFinished);
    static bool getEndProcessing();

    // ieee
private :
    static int m_iIeee;

public :
    static void setIeee(int _iIeee);
    static int getIeee();

    // simp mode
private :
    static int m_iSimpMode;

public :
    static void setSimpMode(int _iSimpMode);
    static int getSimpMode();

    // funcprot mode
private :
    static int m_iFuncprot;

public :
    static void setFuncprot(int _iFuncprot);
    static int getFuncprot();

    // where
public :
    struct WhereEntry
    {
        int m_line;
        int m_absolute_line;
        int m_macro_first_line;
        std::wstring m_name;
        std::wstring m_file_name;
        WhereEntry(int line, int absolute_line, const std::wstring& name, int first_line, const std::wstring& file_name) :
            m_line(line), m_absolute_line(absolute_line), m_name(name), m_macro_first_line(first_line), m_file_name(file_name) {}
    };
    static void where_begin(int _iLineNum, int _iLineLocation, types::Callable* _pCall);
    static void where_end();
    static const std::vector<WhereEntry>& getWhere();
    static void fillWhereError(int _iErrorLine);
    static void resetWhereError();

    static void macroFirstLine_begin(int _iLine);
    static void macroFirstLine_end();
    static int getMacroFirstLines();
    static void setFileNameToLastWhere(const std::wstring& _fileName);
    static void whereErrorToString(std::wostringstream &ostr);
private :
    static std::vector<WhereEntry> m_Where;
    static std::list<WhereEntry> m_WhereError;
    static std::vector<int> m_FirstMacroLine;

    //module called with variable by reference
private :
    static std::list<std::wstring> m_ReferenceModules;
public :
    static bool checkReferenceModule(const std::wstring& _module);
    static void addReferenceModule(const std::wstring& _module);
    static void removeReferenceModule(const std::wstring& _module);
    static std::list<std::wstring> getReferenceModules();

    //analyzer options

private :
    static int m_analyzerOptions;
public :
    static void setAnalyzerOptions(int _val);
    static int getAnalyzerOptions(void);

    //divide by zero
private:
    static bool m_dividebyzero;
public:
    static void setDivideByZero(bool _dividebyzero);
    static bool isDivideByZero(void);

    //mex info
private:
    static std::string mexFunctionName;
public:
    static void setMexFunctionName(const std::string& name);
    static std::string& getMexFunctionName();
};

#endif /* !__CONFIGVARIABLE_HXX__ */
