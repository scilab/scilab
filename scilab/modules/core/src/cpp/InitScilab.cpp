/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
 * Copyright (C) 2013 - Scilab Enterprises - Cedric DELAMARRE
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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

#include "expandPathVariable.h"

extern "C"
{
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
#include "configvariable_interface.h"
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
#include "h5_fileManagement.h"
#include "with_fftw.h"
#include "BrowseVarManager.h"
#include "scicurdir.h"
#include "FileBrowserChDir.h"


#ifdef _MSC_VER
#include "InitializeWindows_tools.h"
#include "TerminateWindows_tools.h"
#include "WndThread.h"
#include "console.h"
#include "InnosetupMutex.h"
#include "MutexClosingScilab.h"
#include "WinConsole.h"
#else
#include "signal_mgmt.h"
#include "initConsoleMode.h"
#endif

#if defined(linux) && defined(__i386__)
#include "setPrecisionFPU.h"
#endif

#include "storeCommand.h"

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
static void Add_Double_Constant(std::wstring _szName, double _dblReal, double _dblImg, bool _bComplex);
static void Add_Poly_Constant(std::wstring _szName, std::wstring _szPolyVar, int _iRank, types::Double * _pdblReal);
static void Add_Boolean_Constant(std::wstring _szName, bool _bBool);
static void Add_String_Constant(std::wstring _szName, const char *_pstString);
static void checkForLinkerErrors(void);

static int batchMain(ScilabEngineInfo* _pSEI);
static int InitializeEnvironnement(void);
static int interactiveMain(ScilabEngineInfo* _pSEI);
static void processCommand(ScilabEngineInfo* _pSEI);
static void stateShow(Parser::ControlStatus status);

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

    return pSEI;
}

int StartScilabEngine(ScilabEngineInfo* _pSEI)
{
    int iMainRet = 0;
    ConfigVariable::setStartProcessing(true);
    ConfigVariable::setForceQuit(false);

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

    checkForLinkerErrors();

#ifdef _MSC_VER
    //get current console window and hide it
    int scilabMode = getScilabMode();
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

    if (_pSEI->iNoJvm == 0) // With JVM
    {
        /* bug 3702 */
        /* tclsci creates a TK window on Windows */
        /* it changes focus on previous windows */
        /* we put InitializeTclTk before InitializeGUI */

        //InitializeTclTk();
        InitializeJVM();
        InitializeGUI();

        /* create needed data structure if not already created */
        loadGraphicModule();

        loadBackGroundClassPath();

        //update %gui to true
        Add_Boolean_Constant(L"%gui", true);
    }

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

    //load gateways
    LoadModules();

    //open a scope for macros
    symbol::Context::getInstance()->scope_begin();
    //execute scilab.start
    if (_pSEI->iNoStart == 0)
    {
        execScilabStartTask(_pSEI->iSerialize != 0);
    }

    //open console scope
    symbol::Context::getInstance()->scope_begin();

    ConfigVariable::setStartProcessing(false);
    int pause = 0;

    //set prompt value
    C2F(setprlev) (&pause);

    ConfigVariable::setPromptMode(0);
    try
    {
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
                StoreConsoleCommand(_pSEI->pstExec);
            }

            if (parser.getTree())
            {
                delete parser.getTree();
                parser.setTree(NULL);
            }
            iMainRet = ConfigVariable::getExitStatus();
        }
        else if (_pSEI->pstFile)
        {
            //-f option execute exec('%s',-1)
            char *pstCommand = (char *)MALLOC(sizeof(char) * (strlen("exec(\"\",-1)") + strlen(_pSEI->pstFile) + 1));
            sprintf(pstCommand, "exec(\"%s\",-1)", _pSEI->pstFile);

            StoreConsoleCommand(pstCommand);
            FREE(pstCommand);
            iMainRet = ConfigVariable::getExitStatus();
            _pSEI->pstExec = NULL;
            _pSEI->pstFile = NULL;
        }
    }
    catch (const ast::ScilabException& se)
    {
        scilabErrorW(se.GetErrorMessage().c_str());
    }

    ConfigVariable::setPromptMode(2);
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
        StoreConsoleCommand(_pSEI->pstExec);
        ThreadManagement::WaitForConsoleExecDoneSignal();
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

    //close console scope
    symbol::Context::getInstance()->scope_end();

    //execute scilab.quit
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
        execScilabQuitTask(_pSEI->iSerialize != 0);
        //call all modules.quit
        EndModules();
    }

    //close macros scope
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

    // cleanup Java dependent features
    saveCWDInPreferences();
    clearScilabPreferences();
    TerminateHistoryManager();

    // stop the JVM
    if (_pSEI->iNoJvm == 0)
    {
        //dynamic_TerminateTclTk();
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
    clearTMPDIR();

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
                    _pSEI->isPrioritary != 0, _pSEI->isConsoleCommand != 0);
    }

    /*
     ** -*- DUMPING STACK AFTER EXECUTION -*-
     */
    if (_pSEI->iDumpStack)
    {
        dumpStackTask(_pSEI->iTimed != 0);
    }
}

// Thread used to parse and execute Scilab command setted in storeCommand
void* scilabReadAndExecCommand(void* param)
{
    int iInterruptibleCmd   = 0;
    int iPrioritaryCmd      = 0;
    int iConsoleCmd         = 0;
    char* command           = NULL;

    ScilabEngineInfo* _pSEI = (ScilabEngineInfo*)param;

    while (ConfigVariable::getForceQuit() == false)
    {
        if (GetCommand(&command, &iInterruptibleCmd, &iPrioritaryCmd, &iConsoleCmd) == 0)
        {
            // command queue is empty
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
        _pSEI->isConsoleCommand = iConsoleCmd;

        ThreadManagement::LockParser();
        Parser parser;
        parser.setParseTrace(_pSEI->iParseTrace != 0);
        parseCommandTask(&parser, _pSEI->iTimed != 0, command);

        if (parser.getExitStatus() == Parser::Failed)
        {
            scilabWriteW(parser.getErrorMessage());
            ThreadManagement::UnlockParser();
            continue;
        }

        _pSEI->pExpTree = parser.getTree();
        ThreadManagement::UnlockParser();

        processCommand(_pSEI);
        FREE(command);
    }

    return NULL;
}

//Thread used to parse and set console commands in storeCommand
void* scilabReadAndStore(void* param)
{
    ThreadManagement::LockStart();
    ThreadManagement::UnlockStart();

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

        if (ConfigVariable::isEmptyLineShow())
        {
            scilabWriteW(L"\n");
        }

        do
        {
            // Show Parser Sate before prompt
            stateShow(controlStatus);

            int pause = ConfigVariable::getPauseLevel();

            //set prompt value
            C2F(setprlev) (&pause);

            char *pstRead = scilabRead();

            if (command == NULL)
            {
                command = pstRead;
                if (strcmp(command, "") == 0)
                {
                    FREE(command);
                    command = NULL;
                    break;
                }
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

        StoreConsoleCommand(command);

        FREE(command);
        command = NULL;

        ThreadManagement::WaitForConsoleExecDoneSignal();
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

    __threadId threadIdConsole;
    __threadKey threadKeyConsole;
    __threadId threadIdCommand;
    __threadKey threadKeyCommand;

    if (_pSEI->iStartConsoleThread)
    {
        ThreadManagement::LockStart();
        // thread to manage console command
        __CreateThreadWithParams(&threadIdConsole, &threadKeyConsole, &scilabReadAndStore, _pSEI);

        // scilabReadAndStore thread must be execute before scilabReadAndExecCommand
        // to be such that the -f command stored is not removed
        // from queue before scilabReadAndStore is waiting for.
        ThreadManagement::WaitForStartPendingSignal();
    }

    // thread to manage command stored
    __CreateThreadWithParams(&threadIdCommand, &threadKeyCommand, &scilabReadAndExecCommand, _pSEI);

    __WaitThreadDie(threadIdCommand);

#ifdef DEBUG
    std::cerr << "To end program press [ENTER]" << std::endl;
#endif

    return ConfigVariable::getExitStatus();
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
    parseFileTask(parser, _pSEI->iTimed != 0, pwstFileName, L"YaSp");

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
    return parser->getExitStatus();
}

/*
** -*- stateView
** Used to show parser state.
** Find if we are stuck within some control structure.
*/
static void stateShow(Parser::ControlStatus status)
{
    switch (status)
    {
        case Parser::WithinFor:
            SetTemporaryPrompt("-for       ->");
            break;
        case Parser::WithinWhile:
            SetTemporaryPrompt("-while     ->");
            break;
        case Parser::WithinIf:
            SetTemporaryPrompt("-if        ->");
            break;
        case Parser::WithinElse:
            SetTemporaryPrompt("-else      ->");
            break;
        case Parser::WithinElseIf:
            SetTemporaryPrompt("-elseif    ->");
            break;
        case Parser::WithinTry:
            SetTemporaryPrompt("-try       ->");
            break;
        case Parser::WithinCatch:
            SetTemporaryPrompt("-catch     ->");
            break;
        case Parser::WithinFunction:
            SetTemporaryPrompt("-function  ->");
            break;
        case Parser::WithinSelect:
            SetTemporaryPrompt("-select    ->");
            break;
        case Parser::WithinCase:
            SetTemporaryPrompt("-case      ->");
            break;
        case Parser::WithinSwitch:
            SetTemporaryPrompt("-switch    ->");
            break;
        case Parser::WithinOtherwise:
            SetTemporaryPrompt("-otherwise ->");
            break;
        case Parser::WithinMatrix:
            SetTemporaryPrompt("- [        ->");
            break;
        case Parser::WithinCell:
            SetTemporaryPrompt("- {        ->");
            break;
        case Parser::WithinBlockComment:
            SetTemporaryPrompt("- /*       ->");
            break;
        case Parser::WithinDots:
            SetTemporaryPrompt("- ...      ->");
            break;
        case Parser::AllControlClosed:
            //ClearTemporaryPrompt();
            break;
    }
}

static int InitializeEnvironnement(void)
{
    SetScilabEnvironment();
    InitializeLocalization();

    ConfigVariable::setConsoleWidth(75);
    ConfigVariable::setFormatSize(10);
    ConfigVariable::setFormatMode(1);
    Add_All_Variables();
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
       Depending on the linking order, sometime, libs are not loaded the right way.
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
    double dbl1 = -1.0;
    double dbl0 = fabs(dbl1 - dbl1);

    Add_Double_Constant(L"%nan", dbl0 / dbl0, 0, false);
}

static void Add_Inf(void)
{
    double dbl1 = 1.0;
    double dbl0 = dbl1 - dbl1;

    Add_Double_Constant(L"%inf", dbl1 / dbl0, 0, false);
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
    Add_Double_Constant(L"%eps", C2F(dlamch) ("p", 1L), 0, false);
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
    Double dblCoef(1, 2);

    dblCoef.set(0, 0, 0);
    dblCoef.set(0, 1, 1);

    Add_Poly_Constant(L"%s", L"s", 1, &dblCoef);
}

static void Add_z(void)
{
    Double dblCoef(1, 2);

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

static void Add_Poly_Constant(wstring _szName, wstring _szPolyVar, int _iRank, Double * _pdbl)
{
    types::Polynom * pVar = new types::Polynom(_szPolyVar, 1, 1, &_iRank);
    SinglePoly *poPoly = pVar->get(0);

    poPoly->setCoef(_pdbl);
    symbol::Context::getInstance()->put(symbol::Symbol(_szName), pVar);
}

static void Add_Double_Constant(wstring _szName, double _dblReal, double _dblImg, bool _bComplex)
{
    types::Double * pVal = new types::Double(1, 1, _bComplex);
    pVal->set(0, 0, _dblReal);
    pVal->setImg(0, 0, _dblImg);
    symbol::Context::getInstance()->put(symbol::Symbol(_szName), pVal);
}

static void Add_Boolean_Constant(wstring _szName, bool _bBool)
{
    types::Bool * pVal = new types::Bool(_bBool);
    symbol::Context::getInstance()->put(symbol::Symbol(_szName), pVal);
}

static void Add_String_Constant(wstring _szName, const char *_pstString)
{
    types::String * ps = new types::String(_pstString);
    symbol::Context::getInstance()->put(symbol::Symbol(_szName), ps);
}
