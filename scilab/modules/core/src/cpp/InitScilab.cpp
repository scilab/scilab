/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
 * Copyright (C) 2013 - Scilab Enterprises - Cedric DELAMARRE
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

#include <string>
#include <libxml/parser.h>

#include "inspector.hxx"
#include "double.hxx"
#include "polynom.hxx"
#include "string.hxx"
#include "bool.hxx"

#include "scilabWrite.hxx"
#include "tasks.hxx"
#include "parser.hxx"
#include "context.hxx"
#include "configvariable.hxx"
#include "filemanager.hxx"
#include "runner.hxx"
#include "visitor_common.hxx"
#include "operations.hxx"
#include "threadmanagement.hxx"
#include "numericconstants.hxx"
#include "expandPathVariable.h"
#include "execvisitor.hxx"
#include "printvisitor.hxx"
#include "debugmanager.hxx"
#include "consoledebugger.hxx"

extern "C"
{
#include <locale.h>
#include "machine.h"
#include "InitializeLocalization.h"
#include "elem_common.h"
#include "InitializeJVM.h"
#include "TerminateJVM.h"
#include "InitializeGUI.h"
#include "graphicModuleLoad.h"
#include "TerminateGraphics.h"
#include "loadBackGroundClassPath.h"
#include "sci_tmpdir.h"
#include "setgetlanguage.h"
#include "InitializeConsole.h"
#include "InitializeHistoryManager.h"
#include "TerminateHistoryManager.h"
#include "prompt.h"
#include "scilabRead.h"
#include "functions_manager.h"
#include "InitScilab.h"
#include "setenvvar.h"
#include "getScilabPreference.h"
#include "saveCWDInPreferences.h"
#include "with_fftw.h"
#include "BrowseVarManager.h"
#include "scicurdir.h"
#include "FileBrowserChDir.h"
#include "InitializePreferences.h"

#ifdef _MSC_VER
#include "InitializeWindows_tools.h"
#include "TerminateWindows_tools.h"
#include "WndThread.h"
#include "console.h"
#include "InnosetupMutex.h"
#include "MutexClosingScilab.h"
#include "WinConsole.h"
#include "SignalManagement.h"
#include "splashScreen.h"
#else
#include "signal_mgmt.h"
#include "initConsoleMode.h"
#endif

#if defined(linux) && defined(__i386__)
#include "setPrecisionFPU.h"
#endif

#include "InitializeTclTk.h"
#include "dynamic_link.h"

    /* Defined without include to avoid useless header dependency */
    extern BOOL isItTheDisabledLib(void);
}

static void Add_i(void);
static void Add_pi(void);
static void Add_eps(void);
static void Add_e(void);
static void Add_s(void);
static void Add_z(void);
static void Add_gui(void);
static void Add_fftw(void);
static void Add_Nan(void);
static void Add_Inf(void);
static void Add_io(void);
static void Add_All_Variables(void);
static void Add_Double_Constant(const std::wstring& _szName, double _dblReal, double _dblImg, bool _bComplex);
static void Add_Poly_Constant(const std::wstring& _szName, const std::wstring& _szPolyVar, int _iRank, types::Double * _pdblReal);
static void Add_Boolean_Constant(const std::wstring& _szName, bool _bBool);
static void Add_String_Constant(const std::wstring& _szName, const char *_pstString);
static void checkForLinkerErrors(void);

static int batchMain(ScilabEngineInfo* _pSEI);
static int InitializeEnvironnement(void);
static int interactiveMain(ScilabEngineInfo* _pSEI);
static void processCommand(ScilabEngineInfo* _pSEI);
static void stateShow(Parser::ControlStatus status);

static void executeDebuggerCommand(std::string cmd);
static void splitOnSpaces(const std::string& command, std::vector<std::string>& vCommand, std::vector<size_t>& vPos);
static void printDebuggerHelp();

using namespace ast;

ScilabEngineInfo* InitScilabEngineInfo()
{
    // Disable all startup flags.
    ScilabEngineInfo* pSEI = (ScilabEngineInfo*)CALLOC(1, sizeof(ScilabEngineInfo));

    //Active default flags
    pSEI->iExecAst = 1;
    pSEI->iNoBanner = 1;

    pSEI->iMultiLine = 0;
    pSEI->isInterruptible = 1;      // by default all thread are interruptible
    pSEI->isPrioritary = 0;         // by default all thread are non-prioritary
    pSEI->iStartConsoleThread = 1;  // used in call_scilab to avoid "prompt" thread execution
    pSEI->iForceQuit = 0;           // management of -quit argument
    pSEI->iTimeoutDelay = 0;        // watchdog delay to avoid deadlocking tests
    pSEI->iCommandOrigin = NONE;

    pSEI->iCodeAction = -1; //default value, no code action ( used on windows by file associations -O -X -P arguments)
    return pSEI;
}

int StartScilabEngine(ScilabEngineInfo* _pSEI)
{
    int iMainRet = 0;
    ConfigVariable::setStartProcessing(true);

    // Update scihome from command line
    if (_pSEI->pstSciHome && strcmp(_pSEI->pstSciHome, "") != 0)
    {
        wchar_t* w = to_wide_string(_pSEI->pstSciHome);
        ConfigVariable::setSCIHOME(w);
        FREE(w);
    }

    // ignore -e argument if the command is empty
    if (_pSEI->pstExec && strcmp(_pSEI->pstExec, "") == 0)
    {
        _pSEI->pstExec = NULL;
    }

    // ignore -quit if -e or -f are not given
    _pSEI->iForceQuit = _pSEI->iForceQuit && (_pSEI->pstExec || _pSEI->pstFile);
    ConfigVariable::setForceQuit(_pSEI->iForceQuit == 1);

    // setup timeout delay
    if (_pSEI->iTimeoutDelay != 0)
    {
        timeout_process_after(_pSEI->iTimeoutDelay);
    }

    /* This bug only occurs under Linux 32 bits
     * See: http://wiki.scilab.org/Scilab_precision
     */
#if defined(linux) && defined(__i386__)
    setFPUToDouble();
#endif

#ifndef _MSC_VER
    /* Management of the signals (seg fault, floating point exception, etc) */
    if (getenv("SCI_DISABLE_EXCEPTION_CATCHING") == NULL)
    {
        base_error_init();
    }
#endif

#if defined(netbsd) || defined(freebsd)
    /* floating point exceptions */
    fpsetmask(0);
#endif

    ThreadManagement::initialize();
    NumericConstants::Initialize();
    checkForLinkerErrors();

#ifdef _MSC_VER

    //get current console window and hide it
    int scilabMode = getScilabMode();

#ifndef _DEBUG
    if (scilabMode == SCILAB_STD && _pSEI->pstFile == NULL && _pSEI->pstExec == NULL)
    {
        splashScreen();
    }
#endif

    if (scilabMode == SCILAB_STD || scilabMode == SCILAB_NW || scilabMode == SCILAB_API)
    {
        CreateScilabHiddenWndThread();
    }

    if (scilabMode == SCILAB_STD)
    {
        //show banner in console window
        CreateScilabConsole(_pSEI->iNoBanner);

        if (_pSEI->iKeepConsole == 0)
        {
            HideScilex(); /* hide console window */
        }
        else
        {
            ShowScilex();
        }
    }
    else
    {
        if (scilabMode == SCILAB_NW || scilabMode == SCILAB_NWNI)
        {
            SaveConsoleColors();
            SaveConsoleFont();
            UpdateConsoleFont();
            if (scilabMode == SCILAB_NW)
            {
                RenameConsole();
                UpdateConsoleColors();
            }
        }
    }

    //create a thread for innosetup to allow reinstall during scilab running
    createInnosetupMutex();
#endif

    ConfigVariable::setWebMode(_pSEI->iWebMode != 0);

    //open scope lvl 0 for gateway from modules and first variables ( SCI, HOME, TMPDIR, ...)
    symbol::Context::getInstance()->scope_begin();

    /* Scilab Startup */
    xmlInitParser();
    InitializeEnvironnement();

    if (_pSEI->pstLang)
    {
        wchar_t *pwstLang = to_wide_string(_pSEI->pstLang);
        setlanguage(pwstLang);
        FREE(pwstLang);
    }

#ifdef _MSC_VER
    InitializeWindows_tools();
#endif

    BOOL bRet = TRUE;
    if (_pSEI->iNoJvm == 0) // With JVM
    {
        bRet = InitializeTclTk();
        InitializeJVM();
        InitializeGUI();

        /* create needed data structure if not already created */
        loadGraphicModule();

        loadBackGroundClassPath();

        //update %gui to true
        Add_Boolean_Constant(L"%gui", true);
    }

    if(bRet == FALSE)
    {
        std::wcerr << "TCL Initialization failed." << std::endl;
        std::wcerr << ConfigVariable::getLastErrorMessage() << std::endl;
        return 1;
    }

    // Make sure the default locale is applied at startup
    setlocale(LC_NUMERIC, "C");

    /* Standard mode -> init Java Console */
    if (_pSEI->iConsoleMode == 0)
    {
        /* Initialize console: lines... */
        InitializeConsole();
    }
    else
    {
#ifndef _MSC_VER
        initConsoleMode(RAW);
#endif
    }

    //set prompt value
    setPreviousLevel(0);

    //load gateways
    if (LoadModules() == false)
    {
        //clear opened files
        FileManager::destroy();
        return 1;
    }

    //variables are needed by loadModules but must be in SCOPE_CONSOLE under protection
    //remove (W)SCI/SCIHOME/HOME/TMPDIR
    types::InternalType* sci = symbol::Context::getInstance()->get(symbol::Symbol(L"SCI"));
    types::InternalType* wsci = symbol::Context::getInstance()->get(symbol::Symbol(L"WSCI"));
    types::InternalType* scihome = symbol::Context::getInstance()->get(symbol::Symbol(L"SCIHOME"));
    types::InternalType* home = symbol::Context::getInstance()->get(symbol::Symbol(L"home"));
    types::InternalType* tmpdir = symbol::Context::getInstance()->get(symbol::Symbol(L"TMPDIR"));

    sci->IncreaseRef();
    if (wsci)
    {
        wsci->IncreaseRef();
    }
    scihome->IncreaseRef();
    home->IncreaseRef();
    tmpdir->IncreaseRef();

    symbol::Context::getInstance()->remove(symbol::Symbol(L"SCI"));
    symbol::Context::getInstance()->remove(symbol::Symbol(L"WSCI"));
    symbol::Context::getInstance()->remove(symbol::Symbol(L"SCIHOME"));
    symbol::Context::getInstance()->remove(symbol::Symbol(L"home"));
    symbol::Context::getInstance()->remove(symbol::Symbol(L"TMPDIR"));

    //open a scope for macros
    symbol::Context::getInstance()->scope_begin();

    //put var in good scope
    Add_All_Variables();
    symbol::Context::getInstance()->put(symbol::Symbol(L"SCI"), sci);
    if (wsci)
    {
        symbol::Context::getInstance()->put(symbol::Symbol(L"WSCI"), wsci);
    }

    symbol::Context::getInstance()->put(symbol::Symbol(L"SCIHOME"), scihome);
    symbol::Context::getInstance()->put(symbol::Symbol(L"home"), home);
    symbol::Context::getInstance()->put(symbol::Symbol(L"TMPDIR"), tmpdir);

    sci->DecreaseRef();
    if (wsci)
    {
        wsci->DecreaseRef();
    }
    scihome->DecreaseRef();
    home->DecreaseRef();
    tmpdir->DecreaseRef();


    symbol::Context::getInstance()->protect();
    //execute scilab.start
    if (_pSEI->iNoStart == 0)
    {
        int ierr = execScilabStartTask(_pSEI->iSerialize != 0);
        if (ierr)
        {
            return ierr;
        }
    }

    //open console scope
    //symbol::Context::getInstance()->scope_begin();

    ConfigVariable::setStartProcessing(false);

    ConfigVariable::setPromptMode(0);
    int iScript = 0;
    if (_pSEI->pstExec)
    {
        //-e option
        Parser parser;
        parseCommandTask(&parser, _pSEI->iTimed != 0, _pSEI->pstExec);

        if (parser.getExitStatus() == Parser::Failed)
        {
            scilabWriteW(parser.getErrorMessage());
        }
        else if (parser.getControlStatus() !=  Parser::AllControlClosed)
        {
            _pSEI->iMultiLine = 1;
        }
        else
        {
            StoreConsoleCommand(_pSEI->pstExec, 0);
        }

        if (parser.getTree())
        {
            delete parser.getTree();
            parser.setTree(NULL);
        }
        iMainRet = ConfigVariable::getExitStatus();
        iScript = 1;

        if (_pSEI->iCodeAction != -1)
        {
            //alloc in main to manage shell interaction
            FREE(_pSEI->pstExec);
            _pSEI->pstExec = NULL;
        }
    }
    else if (_pSEI->pstFile)
    {
        //-f option execute exec('%s',-1)
        char *pstCommand = (char *)MALLOC(sizeof(char) * (strlen("exec(\"\",-1)") + strlen(_pSEI->pstFile) + 1));
        sprintf(pstCommand, "exec(\"%s\",-1)", _pSEI->pstFile);

        StoreConsoleCommand(pstCommand, 0);
        FREE(pstCommand);
        iMainRet = ConfigVariable::getExitStatus();
        _pSEI->pstExec = NULL;
        _pSEI->pstFile = NULL;
        iScript = 1;
    }

    ConfigVariable::setPromptMode(2);
    InitializePreferences(iScript);

    return iMainRet;
}

int RunScilabEngine(ScilabEngineInfo* _pSEI)
{
    if (_pSEI->pstParseFile)
    {
        // Only for parsing test, won't execute anything.
        return batchMain(_pSEI);
    }
    else
    {
        //always run as interactiveMain even after -e or -f option
        return interactiveMain(_pSEI);
    }
}

int ExecExternalCommand(ScilabEngineInfo* _pSEI)
{
    if (_pSEI->pstExec)
    {
        StoreConsoleCommand(_pSEI->pstExec, 1);
        return ConfigVariable::getExitStatus();
    }

    return -1;
}

void StopScilabEngine(ScilabEngineInfo* _pSEI)
{
    ConfigVariable::setEndProcessing(true);
#ifdef _MSC_VER
    /* bug 3672 */
    /* Create a Mutex (closing scilab)
     * used by files association
     */
    createMutexClosingScilab();
#endif

    clearScilabPreferences();

    if (_pSEI->pstFile)
    {
        //-f option execute exec('%s',-1)
        char *pstCommand = (char *)MALLOC(sizeof(char) * (strlen("exec(\"\",-1)") + strlen(_pSEI->pstFile) + 1));
        sprintf(pstCommand, "exec(\"%s\",-1)", _pSEI->pstFile);

        _pSEI->pstExec = pstCommand;
        processCommand(_pSEI);
        FREE(pstCommand);
    }
    else if (_pSEI->iNoStart == 0)
    {
        //execute scilab.quit
        execScilabQuitTask(_pSEI->iSerialize != 0);
        //call all modules.quit
        EndModules();
    }

    // close macros scope
    symbol::Context::getInstance()->scope_end();

    //close gateways scope
    symbol::Context::getInstance()->scope_end();

    //clean context
    symbol::Context::getInstance()->clearAll();
    //destroy context
    symbol::Context::destroyInstance();

#ifndef NDEBUG
    //uncomment to print mem leak log
    //types::Inspector::displayMemleak();
#endif

    //close dynamic linked libraries
    std::vector<ConfigVariable::DynamicLibraryStr*>* pDLLIst = ConfigVariable::getDynamicLibraryList();
    int size = static_cast<int>(pDLLIst->size());
    for (int i = 0; i < size; i++)
    {
        ConfigVariable::DynamicLibraryStr* pStr = ConfigVariable::getDynamicLibrary(i);
        if (pStr)
        {
            DynLibHandle iLib = pStr->hLib;
            ConfigVariable::removeDynamicLibrary(i);
            Sci_dlclose(iLib);
        }
    }

    // cleanup Java dependent features
    saveCWDInPreferences();
    clearScilabPreferences();
    TerminateHistoryManager();

    // stop the JVM
    if (_pSEI->iNoJvm == 0)
    {
        TerminateGraphics();
        TerminateJVM();
    }

    // reset struct to prevent the use of deleted objects
    // when we start scilab again without kill process (ie: call_scilab)
    resetVariableValueDefinedInScilab();

    /* TerminateCorePart2 */

    //clear opened files
    FileManager::destroy();

    /* Remove TMPDIR before exit */
    try
    {
        clearTMPDIR();
    }
    catch (const ast::InternalError& /*ie*/)
    {
        // unable to delete the directory, let's continue
    }


    //Unload dynamic modules
    UnloadModules();

    //destroy function manager
    destroyfunctionManagerInstance();
    /* TerminateCorePart2 end */

    /*
    * Cleanup function for the XML library.
    */
    xmlCleanupParser();

    /* Cleanup the parser state */
    Parser::cleanup();

#ifdef _MSC_VER
    TerminateWindows_tools();
#endif

    /* Reset terminal configuration */
    if (_pSEI->iConsoleMode)
    {
#ifndef _MSC_VER
        initConsoleMode(ATTR_RESET);
#endif
    }

#ifdef _MSC_VER
    /* close mutex (closing scilab)
     * used by files association
     */
    terminateMutexClosingScilab();
#endif

    ConfigVariable::clearLastError();
    ConfigVariable::setEndProcessing(false);
}

static void processCommand(ScilabEngineInfo* _pSEI)
{
    /*
     ** -*- DUMPING TREE -*-
     */
    if (_pSEI->iDumpAst)
    {
        dumpAstTask((ast::Exp*)_pSEI->pExpTree, _pSEI->iTimed != 0);
    }

    /*
     ** -*- PRETTY PRINT TREE -*-
     */
    if (_pSEI->iPrintAst)
    {
        printAstTask((ast::Exp*)_pSEI->pExpTree, _pSEI->iTimed != 0);
    }

    /*
     ** -*- EXECUTING TREE -*-
     */
    if (_pSEI->iExecAst)
    {
        execAstTask((ast::Exp*)_pSEI->pExpTree, _pSEI->iSerialize != 0,
                    _pSEI->iTimed != 0, _pSEI->iAstTimed != 0,
                    _pSEI->iExecVerbose != 0, _pSEI->isInterruptible != 0,
                    _pSEI->isPrioritary != 0, _pSEI->iCommandOrigin);
    }

    /*
     ** -*- DUMPING STACK AFTER EXECUTION -*-
     */
    if (_pSEI->iDumpStack)
    {
        dumpStackTask(_pSEI->iTimed != 0);
    }
}

// Thread used to parse and execute Scilab command set in storeCommand
void* scilabReadAndExecCommand(void* param)
{
    char* command           = NULL;
    int iInterruptibleCmd   = 0;
    int iPrioritaryCmd      = 0;

    command_origin_t iCmdOrigin = NONE;

    ScilabEngineInfo* _pSEI = (ScilabEngineInfo*)param;

    do
    {
        if (GetCommand(&command, &iPrioritaryCmd, &iInterruptibleCmd, &iCmdOrigin) == 0)
        {
            ThreadManagement::WaitForCommandStoredSignal();
            continue;
        }

        // empty command
        if (command == NULL || strlen(command) == 0)
        {
            continue;
        }

        _pSEI->isInterruptible = iInterruptibleCmd;
        _pSEI->isPrioritary = iPrioritaryCmd;
        _pSEI->iCommandOrigin = iCmdOrigin;

        ThreadManagement::LockParser();
        Parser parser;
        parser.setParseTrace(_pSEI->iParseTrace != 0);
        parseCommandTask(&parser, _pSEI->iTimed != 0, command);

        if (parser.getExitStatus() == Parser::Failed)
        {
            scilabWriteW(parser.getErrorMessage());
            ThreadManagement::UnlockParser();
            FREE(command);
            continue;
        }

        _pSEI->pExpTree = parser.getTree();
        ThreadManagement::UnlockParser();

        processCommand(_pSEI);
        FREE(command);
    }
    while (ConfigVariable::getForceQuit() == false);

    return NULL;
}

//Thread used to parse and set console commands in storeCommand
void* scilabReadAndStore(void* param)
{
    Parser::ControlStatus controlStatus = Parser::AllControlClosed;

    char *command = NULL;
    wchar_t* parserErrorMsg = NULL;

    ScilabEngineInfo* _pSEI = (ScilabEngineInfo*)param;

    if (_pSEI->iMultiLine)
    {
        command = _pSEI->pstExec;
    }

    if (isEmptyCommandQueue() == false)
    {
        // unlock main thread
        ThreadManagement::SendStartPendingSignal();

        // Command stored as console command by -f
        // We have to wait this execution before
        // callOnPrompt (ie: onPrompt perform a quit in test_run)
        ThreadManagement::WaitForConsoleExecDoneSignal();
    }

    // unlock main thread
    ThreadManagement::SendStartPendingSignal();

    while (ConfigVariable::getForceQuit() == false)
    {
        callOnPrompt();

        Parser parser;
        parser.setParseTrace(_pSEI->iParseTrace != 0);

        Parser::ParserStatus exitStatus = Parser::Failed;

        if (ConfigVariable::isPrintCompact() == false)
        {
            scilabWriteW(L"\n");
        }

        do
        {
            // Show Parser Sate before prompt
            stateShow(controlStatus);

            int pause = ConfigVariable::getPauseLevel();

            //set prompt value
           setPreviousLevel(pause);

            if (scilabRead() == 0)
            {
                // happens when the return of scilabRead must not be interpreted by Scilab.
                // ie: mscanf, step by step execution (mode 4 or 7)
                ThreadManagement::WaitForConsoleExecDoneSignal();
                continue;
            }

            char* pstRead = ConfigVariable::getConsoleReadStr();
            if (command == NULL)
            {
                command = pstRead;
                if (strcmp(command, "") == 0)
                {
                    FREE(command);
                    command = NULL;
                    break;
                }
#ifdef DEBUG_THREAD
                ThreadManagement::PrintDebugHead();
#endif // DEBUG_THREAD
            }
            else
            {
                if (ConfigVariable::isExecutionBreak())
                {
                    //clean parser state and close opened instruction.
                    if (parser.getControlStatus() != Parser::AllControlClosed)
                    {
                        parser.cleanup();
                        FREE(command);
                        command = NULL;
                        parser.setControlStatus(Parser::AllControlClosed);
                        controlStatus = parser.getControlStatus();
                    }

                    ConfigVariable::resetExecutionBreak();
                    break;
                }
                else
                {
                    //+1 for null termination and +1 for '\n'
                    size_t iLen = strlen(command) + strlen(pstRead) + 2;
                    char *pstNewCommand = (char *)MALLOC(iLen * sizeof(char));

#ifdef _MSC_VER
                    sprintf_s(pstNewCommand, iLen, "%s\n%s", command, pstRead);
#else
                    sprintf(pstNewCommand, "%s\n%s", command, pstRead);
#endif
                    FREE(pstRead);
                    FREE(command);
                    command = pstNewCommand;
                }
            }

            if (ConfigVariable::getEnableDebug())
            {
                executeDebuggerCommand(command);
                FREE(command);
                command = NULL;
                continue;
            }

            ThreadManagement::LockParser();
            parseCommandTask(&parser, _pSEI->iTimed != 0, command);
            controlStatus = parser.getControlStatus();
            exitStatus = parser.getExitStatus();
            parserErrorMsg = parser.getErrorMessage();
            if (parser.getTree())
            {
                delete parser.getTree();
                parser.setTree(NULL);
            }
            ThreadManagement::UnlockParser();
        }
        while (controlStatus != Parser::AllControlClosed);

        if (command == NULL)
        {
            continue;
        }

        if (exitStatus == Parser::Failed)
        {
            FREE(command);
            command = NULL;
            scilabForcedWriteW(parserErrorMsg);
            continue;
        }

        // store the command and wait for this execution ends.
        StoreConsoleCommand(command, 1);

        FREE(command);
        command = NULL;
    }

    // Awake scilabReadAndExecCommand thread in case of scilab exit
    ThreadManagement::SendCommandStoredSignal();
    return NULL;
}

/*
** -*- Interactive Main -*-
*/
static int interactiveMain(ScilabEngineInfo* _pSEI)
{
#ifndef WITH_GUI
#ifndef _MSC_VER
    if (getScilabMode() != SCILAB_NWNI)
    {
        fprintf(stderr, "Scilab was compiled without its GUI and advanced features. Run scilab-cli or use the -nwni option.\n");
        initConsoleMode(ATTR_RESET);
        exit(1);
    }
#endif
#endif

    InitializeHistoryManager();

    if (getScilabMode() != SCILAB_NWNI && getScilabMode() != SCILAB_API)
    {

        char *cwd = NULL;

        int err = 0;

        UpdateBrowseVar();
        cwd = scigetcwd(&err);
        if (cwd)
        {
            FileBrowserChDir(cwd);
            FREE(cwd);
        }
    }

    // -e -quit with parser error, command queue is empty
    if (_pSEI->iForceQuit && isEmptyCommandQueue())
    {
        return 1;
    }

    __threadId threadIdConsole;
    __threadKey threadKeyConsole;
    __threadId threadIdCommand;
    __threadKey threadKeyCommand;

    if (_pSEI->iStartConsoleThread)
    {
        // thread to manage console command
        __CreateThreadWithParams(&threadIdConsole, &threadKeyConsole, &scilabReadAndStore, _pSEI);

        // scilabReadAndStore thread must be execute before scilabReadAndExecCommand
        // to be such that the -f command stored is not removed
        // from queue before scilabReadAndStore is waiting for.
        ThreadManagement::WaitForStartPendingSignal();
    }

    // thread to manage command stored
    __CreateThreadWithParams(&threadIdCommand, &threadKeyCommand, &scilabReadAndExecCommand, _pSEI);

#ifdef DEBUG_THREAD
    ThreadManagement::SetThreadKey( __GetCurrentThreadKey(), threadKeyCommand, threadKeyConsole);
#endif // DEBUG_THREAD

    int iRet = 0;
    do
    {
        // wait for available runner
        ThreadManagement::WaitForRunMeSignal();

        try
        {
            iRet = StaticRunner::launch();
        }
        catch (const ast::InternalAbort& /*ia*/)
        {
            // go out when exit/quit is called
            iRet = ConfigVariable::getExitStatus();
        }
        catch (const ast::RecursionException& /*re*/)
        {
            // go out when exit/quit is called
            iRet = 1;
        }

        ThreadManagement::SendAwakeRunnerSignal();
    }
    while (ConfigVariable::getForceQuit() == false);

    return iRet;
}

/*
** -*- Batch Main -*-
*/
static int batchMain(ScilabEngineInfo* _pSEI)
{
    /*
     ** -*- PARSING -*-
     */
    Parser *parser = new Parser();

    parser->setParseTrace(_pSEI->iParseTrace != 0);

    wchar_t *pwstFileName = to_wide_string(_pSEI->pstParseFile);

    /*
     ** -*- PARSING -*-
     */
    parseFileTask(parser, _pSEI->iTimed != 0, pwstFileName, L"scilab 6");

    /*
     ** -*- DUMPING TREE -*-
     */
    if (_pSEI->iDumpAst)
    {
        dumpAstTask(parser->getTree(), _pSEI->iTimed != 0);
    }

    if (parser->getExitStatus() == Parser::Succeded)
    {
        /*
         ** -*- PRETTY PRINT TREE -*-
         */
        if (_pSEI->iPrintAst)
        {
            printAstTask(parser->getTree(), _pSEI->iTimed != 0);
        }

    }
    else
    {
        scilabWriteW(parser->getErrorMessage());
    }

#ifdef DEBUG
    std::cerr << "To end program press [ENTER]" << std::endl;
#endif
    int ret = parser->getExitStatus();
    delete parser;
    FREE(pwstFileName);
    return ret;
}

/*
** -*- stateView
*/
static void stateShow(Parser::ControlStatus status)
{
    if (status != Parser::AllControlClosed)
    {
        SetTemporaryPrompt("  > ");
    }
}

static int InitializeEnvironnement(void)
{
    SetScilabEnvironment();
    InitializeLocalization();

    ConfigVariable::setConsoleWidth(75);
    ConfigVariable::setFormatSize(10);
    ConfigVariable::setFormatMode(1);
    //Add_All_Variables();
    FileManager::initialize();
    initOperationArray();
    return 0;
}

/*
 * Private function to check any linker errors
 */

static void checkForLinkerErrors(void)
{
#ifndef _MSC_VER
    /*
       Depending on the linking order, sometimes, libs are not loaded the right way.
       This can cause painful debugging tasks for packager or developer, we are
       doing the check to help them.
    */
#define LINKER_ERROR_1 "Scilab startup function detected that the function proposed to the engine is the wrong one. Usually, it comes from a linker problem in your distribution/OS.\n"
#define LINKER_ERROR_2 "If you do not know what it means, please report a bug on http://bugzilla.scilab.org/. If you do, you probably know that you should change the link order in SCI/modules/Makefile.am\n"

    if (getScilabMode() != SCILAB_NWNI && getScilabMode() != SCILAB_API)
    {
        if (isItTheDisabledLib())
        {
            fprintf(stderr, LINKER_ERROR_1);
            fprintf(stderr, "Here, Scilab should have 'libscijvm' defined but gets 'libscijvm-disable' instead.\n");
            fprintf(stderr, LINKER_ERROR_2);
            exit(1);
        }
    }
    else
    {
        /* NWNI mode */
        if (!isItTheDisabledLib())
        {
            fprintf(stderr, LINKER_ERROR_1);
            fprintf(stderr, "Here, Scilab should have 'libscijvm-disable' defined but gets 'libscijvm' instead.\n");
            fprintf(stderr, LINKER_ERROR_2);
            exit(1);
        }
    }
#undef LINKER_ERROR_1
#undef LINKER_ERROR_2
#endif
}

static void Add_All_Variables(void)
{
    Add_pi();
    Add_eps();
    Add_e();
    Add_i();
    Add_s();
    Add_z();
    Add_gui();
    Add_fftw();
    Add_Nan();
    Add_Inf();
    Add_io();
}

static void Add_Nan(void)
{
    Add_Double_Constant(L"%nan", NumericConstants::nan, 0, false);
}

static void Add_Inf(void)
{
    Add_Double_Constant(L"%inf", NumericConstants::infinity, 0, false);
}

static void Add_gui(void)
{
    Add_Boolean_Constant(L"%gui", false);
}

static void Add_fftw(void)
{
    Add_Boolean_Constant(L"%fftw", withfftw() == 1);
}

static void Add_pi(void)
{
    Add_Double_Constant(L"%pi", M_PI, 0, false);
}

static void Add_eps(void)
{
    Add_Double_Constant(L"%eps", NumericConstants::eps_machine, 0, false);
}

static void Add_e(void)
{
    Add_Double_Constant(L"%e", 2.71828182845904530, 0, false);
}

static void Add_i(void)
{
    Add_Double_Constant(L"%i", 0, 1, true);
}

static void Add_s(void)
{
    types::Double dblCoef(1, 2);

    dblCoef.set(0, 0, 0);
    dblCoef.set(0, 1, 1);

    Add_Poly_Constant(L"%s", L"s", 1, &dblCoef);
}

static void Add_z(void)
{
    types::Double dblCoef(1, 2);

    dblCoef.set(0, 0, 0);
    dblCoef.set(0, 1, 1);

    Add_Poly_Constant(L"%z", L"z", 1, &dblCoef);
}

static void Add_io(void)
{
    types::Double * pVal = new types::Double(1, 2);
    pVal->set(0, 5);
    pVal->set(1, 6);
    symbol::Context::getInstance()->put(symbol::Symbol(L"%io"), pVal);
}

static void Add_Poly_Constant(const std::wstring& _szName, const std::wstring& _szPolyVar, int _iRank, types::Double * _pdbl)
{
    types::Polynom * pVar = new types::Polynom(_szPolyVar, 1, 1, &_iRank);
    types::SinglePoly *poPoly = pVar->get(0);

    poPoly->setCoef(_pdbl);
    symbol::Context::getInstance()->put(symbol::Symbol(_szName), pVar);
}

static void Add_Double_Constant(const std::wstring& _szName, double _dblReal, double _dblImg, bool _bComplex)
{
    types::Double * pVal = new types::Double(1, 1, _bComplex);
    pVal->set(0, 0, _dblReal);
    pVal->setImg(0, 0, _dblImg);
    symbol::Context::getInstance()->put(symbol::Symbol(_szName), pVal);
}

static void Add_Boolean_Constant(const std::wstring& _szName, bool _bBool)
{
    types::Bool * pVal = new types::Bool(_bBool);
    symbol::Context::getInstance()->put(symbol::Symbol(_szName), pVal);
}

static void Add_String_Constant(const std::wstring& _szName, const char *_pstString)
{
    types::String * ps = new types::String(_pstString);
    symbol::Context::getInstance()->put(symbol::Symbol(_szName), ps);
}

// manage debugger commands
static void executeDebuggerCommand(std::string _command)
{
    debugger::DebuggerManager* manager = debugger::DebuggerManager::getInstance();
    std::vector<std::string> vCommand;
    std::vector<size_t> vPos;
    splitOnSpaces(_command, vCommand, vPos);

    std::string cmd = vCommand[0];
    int iSize = vCommand.size();

    // check prefix
    if (cmd.compare("e")    == 0 ||
        cmd.compare("exec") == 0 ||
        cmd.compare("r")    == 0 ||
        cmd.compare("run")  == 0)
    {
        if (manager->isInterrupted())
        {
            // try to execute a command when execution is stopped on a breakpoint
            sciprint(_("Debugger is on a breakpoint\n"));
            sciprint(_("(c)ontinue or (a)bort current execution before execute a new command\n"));
            vCommand.clear();
            return;
        }

        if(iSize == 1)
        {
            sciprint(_("%s: Command missing.\n"), "run");
            sciprint("use 'h' for more information\n\n");
            vCommand.clear();
            return;
        }

        // execute a command
        char* error = manager->execute(_command.erase(0, _command.find(" ")).data());
        if(error)
        {
            sciprint("Debugger execution failed\n\n%s\n", error);
            vCommand.clear();
            return;
        }
    }
    else if(cmd.compare("d")     == 0 ||
            cmd.compare("disp")  == 0 ||
            cmd.compare("p")     == 0 ||
            cmd.compare("print") == 0)
    {
        if(iSize < 2)
        {
            sciprint(_("%s: Wrong number of input arguments: %d expected.\n"), "print", 2);
            sciprint("use 'h' for more information\n\n");
            vCommand.clear();
            return;
        }

        manager->print(vCommand[1]);
    }
    else if(cmd.compare("a")      == 0 ||
            cmd.compare("abort")  == 0)
    {
        manager->abort();
    }
    else if(cmd.compare("c")         == 0 ||
            cmd.compare("continue")  == 0 ||
            cmd.compare("resume")    == 0)
    {
        if (manager->isInterrupted() == false)
        {
            // cannot execute this comment if the execution is not stopped
            sciprint("debugger is not on a breakpoint\n");
            vCommand.clear();
            return;
        }

        manager->resume();
    }
    else if(cmd.compare("b")           == 0 ||
            cmd.compare("break")       == 0 ||
            cmd.compare("breakpoint")  == 0)
    {
        if(iSize < 2)
        {
            sciprint(_("%s: Wrong number of input arguments: %d to %d expected.\n"), "breakpoint", 1, 3);
            sciprint("use 'h' for more information\n\n");
            vCommand.clear();
            return;
        }

        debugger::Breakpoint* bp = new debugger::Breakpoint();

        // set file or function name
        bool isFile = vCommand[1].find_first_of("\\./") != std::string::npos;
        isFile ? bp->setFileName(vCommand[1]) : bp->setFunctionName(vCommand[1]);

        // set file or function line
        if(iSize > 2)
        {
            try
            {
                int iFileFuncLine = std::stoi(vCommand[2].data());
                isFile ? bp->setFileLine(iFileFuncLine) : bp->setMacroLine(iFileFuncLine);
            }
            catch(std::invalid_argument e) // std::stoi
            {
                sciprint(_("%s: Wrong type for input argument #%d: Integer expected.\n"), "breakpoint", 2);
                sciprint("use 'h' for more information\n\n");
                vCommand.clear();
                return;
            }
        }

        // set condition
        if(iSize > 3)
        {
            char* error = bp->setCondition(_command.substr(vPos[3], _command.length() - vPos[3]));
            if(error)
            {
                delete bp;
                bp = nullptr;
                sciprint("parsing condition failed\n\n%s\n", error);
                FREE(error);
                vCommand.clear();
                return;
            }
        }

        manager->addBreakPoint(bp);
    }
    else if(cmd.compare("w")     == 0 ||
            cmd.compare("bt")    == 0 ||
            cmd.compare("where") == 0)
    {
        if (manager->isInterrupted() == false)
        {
            // cannot execute this comment if the execution is not stopped
            sciprint("debugger is not on a breakpoint\n");
            vCommand.clear();
            return;
        }

        sciprint("%s\n", _("callstack:"));
        debugger::DebuggerManager::CallStack callstack = manager->getCallStack();
        int i = 0;
        sciprint("#%-5d%s\n", i++, callstack.exp.data());
        for (auto row : callstack.stack)
        {
            if(row.functionLine < 0)
            {
                sciprint(_("#%-5d%s"), i++, row.functionName.data());
            }
            else
            {
                sciprint(_("#%-5d%s (line %d)"), i++, row.functionName.data(), row.functionLine);
            }

            if(row.hasFile)
            {
                sciprint("  %s:%d", row.fileName.data(), row.fileLine);
            }

            sciprint("\n");
        }
    }
    else if(cmd.compare("disable") == 0)
    {
        if(iSize == 1)
        {
            manager->disableAllBreakPoints();
        }
        else
        {
            try
            {
                int bp = std::stoi(vCommand[1].data());
                if (manager->getBreakPoint(bp) == NULL)
                {
                    sciprint(_("%s: Unable to retrieve information about breakpoint %d.\n"), "disable", bp);
                    sciprint("use 'h' for more information\n\n");
                    vCommand.clear();
                    return;
                }
                manager->disableBreakPoint(bp);
            }
            catch(std::invalid_argument e) // std::stoi
            {
                sciprint(_("%s: Wrong type for input argument #%d: Integer expected.\n"), "disable", 1);
                sciprint("use 'h' for more information\n\n");
                vCommand.clear();
                return;
            }
        }
    }
    else if(cmd.compare("enable") == 0)
    {
        if(iSize == 1)
        {
            manager->enableAllBreakPoints();
        }
        else
        {
            try
            {
                int bp = std::stoi(vCommand[1].data());
                if (manager->getBreakPoint(bp) == NULL)
                {
                    sciprint(_("%s: Unable to retrieve information about breakpoint %d.\n"), "enable", bp);
                    sciprint("use 'h' for more information\n\n");
                    vCommand.clear();
                    return;
                }
                manager->enableBreakPoint(bp);
            }
            catch(std::invalid_argument e) // std::stoi
            {
                sciprint(_("%s: Wrong type for input argument #%d: Integer expected.\n"), "enable", 1);
                sciprint("use 'h' for more information\n\n");
                vCommand.clear();
                return;
            }
        }
    }
    else if(cmd.compare("del")   == 0 ||
            cmd.compare("delete") == 0)
    {
        if(iSize == 1)
        {
            manager->removeAllBreakPoints();
        }
        else
        {
            try
            {
                int bp = std::stoi(vCommand[1].data());
                if (manager->getBreakPoint(bp) == NULL)
                {
                    sciprint(_("%s: Unable to retrieve information about breakpoint %d.\n"), "delete", bp);
                    sciprint("use 'h' for more information\n\n");
                    vCommand.clear();
                    return;
                }
                manager->removeBreakPoint(bp);
            }
            catch(std::invalid_argument e) // std::stoi
            {
                sciprint(_("%s: Wrong type for input argument #%d: Integer expected.\n"), "delete", 1);
                sciprint("use 'h' for more information\n\n");
                vCommand.clear();
                return;
            }
        }
    }
    else if(cmd.compare("h")     == 0 ||
            cmd.compare("help")  == 0)
    {
        if(cmd.compare("help") == 0 &&
          (ConfigVariable::getScilabMode() == SCILAB_NW || ConfigVariable::getScilabMode() == SCILAB_STD))
        {
            StorePrioritaryCommand("help debug");
            vCommand.clear();
            return;
        }

        printDebuggerHelp();
    }
    else if(cmd.compare("l")     == 0 ||
            cmd.compare("list")  == 0)
    {
        if (manager->isInterrupted() == false)
        {
            // cannot execute this comment if the execution is not stopped
            sciprint("debugger is not on a breakpoint\n");
            vCommand.clear();
            return;
        }

        std::wostringstream ostr;
        ast::PrintVisitor pp(ostr, true, true, true);
        manager->getExp()->accept(pp);
        sciprint(_("%ls"), ostr.str().data());
    }
    else if(cmd.compare("i")      == 0 ||
            cmd.compare("in")     == 0 ||
            cmd.compare("stepin") == 0)
    {
        if (manager->isInterrupted() == false)
        {
            // cannot execute this comment if the execution is not stopped
            sciprint("debugger is not on a breakpoint\n");
            vCommand.clear();
            return;
        }

        manager->setStepIn();
        manager->resume();
    }
    else if(cmd.compare("o")         == 0 ||
            cmd.compare("out")       == 0 ||
            cmd.compare("stepout")   == 0)
    {
        if (manager->isInterrupted() == false)
        {
            // cannot execute this comment if the execution is not stopped
            sciprint("debugger is not on a breakpoint\n");
            vCommand.clear();
            return;
        }

        manager->setStepOut();
        manager->resume();
    }
    else if(cmd.compare("n")         == 0 ||
            cmd.compare("next")      == 0 ||
            cmd.compare("stepnext")  == 0)
    {
        if (manager->isInterrupted() == false)
        {
            // cannot execute this comment if the execution is not stopped
            sciprint("debugger is not on a breakpoint\n");
            vCommand.clear();
            return;
        }

        manager->setStepNext();
        manager->resume();
    }
    else if(cmd.compare("q")     == 0 ||
            cmd.compare("exit")  == 0 ||
            cmd.compare("quit")  == 0)
    {
        // quit debugger
        manager->sendQuit();
        ConfigVariable::setEnableDebug(false);
        ConfigVariable::setDefaultVisitor(new ast::ExecVisitor());
        StoreConsoleCommand("abort", 1);
        manager->removeDebugger("console");
    }
    else if(cmd.compare("s")     == 0 ||
            cmd.compare("show")  == 0)
    {
        if(iSize == 1)
        {
            manager->show(-1);
        }
        else
        {
            try
            {
                int bp = std::stoi(vCommand[1].data());
                if (manager->getBreakPoint(bp) == NULL)
                {
                    sciprint(_("%s: Unable to retrieve information about breakpoint %d.\n"), "show", bp);
                    sciprint("use 'h' for more information\n\n");
                    vCommand.clear();
                    return;
                }
                manager->show(bp);
            }
            catch(std::invalid_argument e) // std::stoi
            {
                sciprint(_("%s: Wrong type for input argument #%d: Integer expected.\n"), "show", 1);
                sciprint("use 'h' for more information\n\n");
                vCommand.clear();
                return;
            }
        }
    }
    else
    {
        // not a debugger command
        sciprint("Unknown command \"%s\".\n\n", _command.data());
        sciprint("use 'h' for more information\n\n");
    }

    vCommand.clear();
}

static void splitOnSpaces(const std::string& command, std::vector<std::string>& vCommand, std::vector<size_t>& vPos)
{
    size_t pos = command.find(' ');
    size_t lastPos = 0;

    while(pos != std::string::npos)
    {
        if(pos - lastPos)
        {
            vCommand.push_back(command.substr(lastPos, pos - lastPos));
            vPos.push_back(lastPos);
        }

        lastPos = pos + 1;
        pos = command.find(' ', lastPos);
    }

    if(lastPos != command.length())
    {
        vCommand.push_back(command.substr(lastPos, std::min(pos, command.size()) - lastPos + 1 ) );
        vPos.push_back(lastPos);
    }
}

static void printDebuggerHelp()
{
    sciprint(_("debug commands : \n"));
    sciprint("  h                            : %s.\n", _("show this help"));
    sciprint("  help                         : %s.\n", _("open debug documentation page"));
    sciprint("\n");
    sciprint("  (q)uit                       : %s.\n", _("stop debugging"));
    sciprint("  (w)here or bt                : %s.\n", _("show callstack"));
    sciprint("\n");
    sciprint("  (e)xec cmd                   : %s.\n", _("execute cmd"));
    sciprint("  (r)un cmd                    : %s.\n", _("execute cmd"));
    sciprint("\n");
    sciprint("  (d)isp var                   : %s.\n", _("display variable"));
    sciprint("  (p)rint var                  : %s.\n", _("display variable"));
    sciprint("\n");
    sciprint("  (c)ontinue                   : %s.\n", _("continue execution"));
    sciprint("  (a)bort                      : %s.\n", _("abort execution"));
    sciprint("  step(n)ext or next           : %s.\n", _("continue to next statement"));
    sciprint("  step(i)n or in               : %s.\n", _("step into function"));
    sciprint("  step(o)ut or out             : %s.\n", _("step outside function"));
    sciprint("\n");
    sciprint("  (b)reakpoint or break\n     func [line [\"condition\"]] : %s.\n", _("add a breakpoint"));
    sciprint("  (del)ete                     : %s.\n", _("delete all breakpoints"));
    sciprint("  (del)ete n                   : %s.\n", _("delete a specific breakpoint"));
    sciprint("  enable                       : %s.\n", _("enable all breakpoints"));
    sciprint("  enable n                     : %s.\n", _("enable a specific breakpoint"));
    sciprint("  disable                      : %s.\n", _("disable all breakpoints"));
    sciprint("  disable n                    : %s.\n", _("disable a specific breakpoint"));
    sciprint("  (s)how                       : %s.\n", _("show all breakpoints"));
    sciprint("  (s)how n                     : %s.\n", _("show a specific breakpoint"));
    sciprint("\n");
    sciprint(_("  for more details, show help page.\n"));
}
