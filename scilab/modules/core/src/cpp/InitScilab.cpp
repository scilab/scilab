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


#include "double.hxx"
#include "polynom.hxx"
#include "string.hxx"
#include "bool.hxx"

#include "scilabWrite.hxx"
#include "tasks.hxx"
#include "parser.hxx"
#include "context.hxx"
#include "configvariable.hxx"
#include "setenvvar.hxx"
#include "filemanager.hxx"
#include "runner.hxx"

extern "C"
{
#include "machine.h"
#include "InitializeLocalization.h"
#include "elem_common.h"
#include "LaunchScilabSignal.h"
#include "InitializeJVM.h"
#include "TerminateJVM.h"
#include "InitializeGUI.h"
#include "graphicModuleLoad.h"
#include "TerminateGraphics.h"
#include "loadBackGroundClassPath.h"
#include "sci_tmpdir.h"
#include "sci_mode.h"
#include "setgetlanguage.h"
#include "InitializeConsole.h"
#include "InitializeHistoryManager.h"
#include "TerminateHistoryManager.h"
#include "prompt.h"
#include "scilabRead.h"
#include "functions_manager.h"
#include "InitScilab.h"
#include "getScilabPreference.h"

#ifdef _MSC_VER
#include "InitializeWindows_tools.h"
#include "TerminateWindows_tools.h"
#else
#include "signal_mgmt.h"
#include "initConsoleMode.h"
#endif

#if defined(linux) && defined(__i386__)
#include "setPrecisionFPU.h"
#endif

    /* Defined without include to avoid useless header dependency */
    extern BOOL isItTheDisabledLib(void);
}

static void Add_i(void);
static void Add_pi(void);
static void Add_eps(void);
static void Add_e(void);
static void Add_s(void);
static void Add_z(void);
static void Add_true(void);
static void Add_false(void);
static void Add_Nan(void);
static void Add_Inf(void);
static void Add_WITH_DEMOS(void);      //temporary variable
static void Add_All_Variables(void);
static void Add_Double_Constant(std::wstring _szName, double _dblReal, double _dblImg, bool _bComplex);
static void Add_Poly_Constant(std::wstring _szName, std::wstring _szPolyVar, int _iRank, types::Double * _pdblReal);
static void Add_Boolean_Constant(std::wstring _szName, bool _bBool);
static void Add_String_Constant(std::wstring _szName, const char *_pstString);
static void checkForLinkerErrors(void);

static int batchMain(char *pstFileName,
                     bool _bParseTrace = false,
                     bool _bTimed = false,
                     bool _bDumpAst = false,
                     bool _bPrintAst = false);
static int InitializeEnvironnement(void);
static int interactiveMain(void);
static Parser::ControlStatus processCommand(char *_pstCommand,
        bool _bParseTrace = false,
        bool _bTimed = false,
        bool _bDumpAst = false,
        bool _bPrintAst = false,
        bool _bExecAst = true,
        bool _bDumpStack = false,
        bool _bASTTimed = false,
        bool _bExecVerbose = false);
static void stateShow(Parser::ControlStatus status);

int StartScilabEngine(char* _pstExec, char* _pstFile, char* _pstLang, int _iNoStart, int _iJvm, int _iConsole)
{
    int iMainRet = 0;

    InitializeLaunchScilabSignal();

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

    Runner::init();

    checkForLinkerErrors();

    /* Scilab Startup */
    InitializeEnvironnement();

    if (_pstLang)
    {
        wchar_t *pwstLang = to_wide_string(_pstLang);
        setlanguage(pwstLang);
        FREE(pwstLang);
    }

#ifdef _MSC_VER
    InitializeWindows_tools();
#endif

    if (_iJvm)
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
    }

    /* Standard mode -> init Java Console */
    if (!_iConsole)
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

    LoadModules(_iNoStart);

    //execute scilab.start
    if (_iNoStart == 0)
    {
        execScilabStartTask();
    }

    int pause = 0;

    //set prompt value
    C2F(setprlev) (&pause);

    ConfigVariable::setPromptMode(0);
    try
    {
        if (_pstExec)
        {
            //-e option

            processCommand(_pstExec);
            iMainRet = ConfigVariable::getExitStatus();
        }
        else if (_pstFile)
        {
            //-f option execute exec('%s',-1)
            char *pstCommand = (char *)MALLOC(sizeof(char) * (strlen("exec(\"\",-1)") + strlen(_pstFile) + 1));

            sprintf(pstCommand, "exec(\"%s\",-1)", _pstFile);
            processCommand(pstCommand);
            FREE(pstCommand);
            iMainRet = ConfigVariable::getExitStatus();
        }
    }
    catch (ScilabException se)
    {
        scilabErrorW(se.GetErrorMessage().c_str());
    }

    ConfigVariable::setStartFinished(true);
    ConfigVariable::setPromptMode(2);
    return iMainRet;
}

int RunScilabEngine(char* _pstFile)
{
    if (_pstFile)
    {
        // Only for parsing test, won't execute anything.
        return batchMain(_pstFile);
    }
    else
    {
        //always run as interactiveMain even after -e or -f option
        return interactiveMain();
    }
}

int ExecExternalCommand(char* _pstExec)
{
    if (_pstExec)
    {
        processCommand(_pstExec);
        return ConfigVariable::getExitStatus();
    }

    return -1;
}

void StopScilabEngine(char* _pstFile, int _iNoStart, int _iConsole)
{
    clearScilabPreferences();

    //execute scilab.quit
    if (_pstFile)
    {
        //-f option execute exec('%s',-1)
        char *pstCommand = (char *)MALLOC(sizeof(char) * (strlen("exec(\"\",-1)") + strlen(_pstFile) + 1));

        sprintf(pstCommand, "exec(\"%s\",-1)", _pstFile);
        processCommand(pstCommand);
        FREE(pstCommand);
    }
    else if (_iNoStart == 0)
    {
        execScilabQuitTask();
    }


    //close main scope
    symbol::Context::getInstance()->scope_end();


    if (getScilabMode() != SCILAB_NWNI)
    {
        //dynamic_TerminateTclTk();
        TerminateGraphics();
        TerminateJVM();
    }


    //clear opened files
    FileManager::destroy();

    /*
    * Cleanup function for the XML library.
    */
    xmlCleanupParser();

    /* Remove TMPDIR before exit */
    clearTMPDIR();


#ifdef _MSC_VER
    TerminateWindows_tools();
#endif

    /* Reset terminal configuration */
    if (_iConsole)
    {
#ifndef _MSC_VER
        initConsoleMode(ATTR_RESET);
#endif
    }


}

static Parser::ControlStatus processCommand(char *_pstCommand,
        bool _bParseTrace,
        bool _bTimed,
        bool _bDumpAst,
        bool _bPrintAst,
        bool _bExecAst,
        bool _bDumpStack,
        bool _bASTTimed,
        bool _bExecVerbose)
{
    Parser *parser = new Parser();

    parser->setParseTrace(_bParseTrace);
    if (strcmp(_pstCommand, "") != 0)
    {
        wchar_t *pwstCommand = to_wide_string(_pstCommand);

        /*
         ** -*- PARSING -*-
         */
        parseCommandTask(parser, _bTimed, pwstCommand);

        /*
         ** -*- DUMPING TREE -*-
         */
        if (_bDumpAst == true)
        {
            dumpAstTask(parser->getTree(), _bTimed);
        }

        if (parser->getExitStatus() == Parser::Succeded)
        {
            /*
             ** -*- PRETTY PRINT TREE -*-
             */
            if (_bPrintAst == true)
            {
                printAstTask(parser->getTree(), _bTimed);
            }

            /*
             ** -*- EXECUTING TREE -*-
             */
            if (_bExecAst == true)
            {
                //before calling YaspReader, try to call %onprompt function
                callOnPrompt();
                execAstTask(parser->getTree(), _bTimed, _bASTTimed, _bExecVerbose);
            }

            /*
             ** -*- DUMPING STACK AFTER EXECUTION -*-
             */
            if (_bDumpStack == true)
            {
                dumpStackTask(_bTimed);
            }
        }
        else if (parser->getExitStatus() == Parser::Failed && parser->getControlStatus() == Parser::AllControlClosed)
        {
            if (_bExecAst == true)
            {
                //before calling YaspReader, try to call %onprompt function
                callOnPrompt();
            }

            scilabWriteW(parser->getErrorMessage());
        }

        FREE(pwstCommand);
    }
    else
    {
        if (_bExecAst == true)
        {
            //before calling YaspReader, try to call %onprompt function
            callOnPrompt();
        }
    }
    return parser->getControlStatus();
}

/*
** -*- Interactive Main -*-
*/
static int interactiveMain(void)
{
    int pause = 0;
    char *command = NULL;

    Parser::ControlStatus controlStatus = Parser::AllControlClosed;

#ifndef WITH_GUI
#ifndef _MSC_VER
    if (getScilabMode() != SCILAB_NWNI)
    {
        fprintf(stderr, "Scilab was compiled without its GUI and advanced features. Run scilab-cli or us the -nwni option.\n");
        initConsoleMode(ATTR_RESET);
        exit(1);
    }
#endif
#endif

    InitializeHistoryManager();

    //before calling reader, try to call %onprompt function
    callOnPrompt();

    while (!ConfigVariable::getForceQuit())
    {
        // Show Parser Sate before prompt
        stateShow(controlStatus);

        pause = ConfigVariable::getPauseLevel();

        //set prompt value
        C2F(setprlev) (&pause);

        if (controlStatus == Parser::AllControlClosed)
        {
            if (command)
            {
                FREE(command);
                command = NULL;
            }
            scilabWriteW(L"\n");
            command = scilabRead();
        }
        else
        {
            char *pstRead = scilabRead();

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

        controlStatus = processCommand(command);
    }
#ifdef DEBUG
    std::cerr << "To end program press [ENTER]" << std::endl;
#endif
    return ConfigVariable::getExitStatus();
}

/*
** -*- Batch Main -*-
*/
static int batchMain(char *pstFileName,
                     bool _bParseTrace,
                     bool _bTimed,
                     bool _bDumpAst,
                     bool _bPrintAst)
{
    /*
     ** -*- PARSING -*-
     */
    Parser *parser = new Parser();

    parser->setParseTrace(_bParseTrace);

    wchar_t *pwstFileName = to_wide_string(pstFileName);

    /*
     ** -*- PARSING -*-
     */
    parseFileTask(parser, _bTimed, pwstFileName, L"YaSp");

    /*
     ** -*- DUMPING TREE -*-
     */
    if (_bDumpAst == true)
    {
        dumpAstTask(parser->getTree(), _bTimed);
    }

    if (parser->getExitStatus() == Parser::Succeded)
    {
        /*
         ** -*- PRETTY PRINT TREE -*-
         */
        if (_bPrintAst == true)
        {
            printAstTask(parser->getTree(), _bTimed);
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
            ClearTemporaryPrompt();
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

    if (getScilabMode() != SCILAB_NWNI)
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
    Add_true();
    Add_false();
    Add_Nan();
    Add_Inf();
    Add_WITH_DEMOS();
}

static void Add_WITH_DEMOS(void)
{
    Add_Boolean_Constant(L"WITH_DEMOS", false);
}

static void Add_Nan(void)
{
    double dbl1 = 1.0;
    double dbl0 = dbl1 - dbl1;

    Add_Double_Constant(L"%nan", dbl0 / dbl0, 0, false);
}

static void Add_Inf(void)
{
    double dbl1 = 1.0;
    double dbl0 = dbl1 - dbl1;

    Add_Double_Constant(L"%inf", dbl1 / dbl0, 0, false);
}

static void Add_false(void)
{
    Add_Boolean_Constant(L"%f", false);
}

static void Add_true(void)
{
    Add_Boolean_Constant(L"%t", true);
}

static void Add_pi(void)
{
    Add_Double_Constant(L"%pi", 3.1415926535897931159980, 0, false);
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

    Add_Poly_Constant(L"%s", L"s", 2, &dblCoef);
}

static void Add_z(void)
{
    Double dblCoef(1, 2);

    dblCoef.set(0, 0, 0);
    dblCoef.set(0, 1, 1);

    Add_Poly_Constant(L"%z", L"z", 2, &dblCoef);
}

static void Add_Poly_Constant(wstring _szName, wstring _szPolyVar, int _iRank, Double * _pdbl)
{
    types::Polynom * pVar = new types::Polynom(_szPolyVar, 1, 1, &_iRank);
    SinglePoly *poPoly = pVar->get(0, 0);

    poPoly->setCoef(_pdbl);
    symbol::Context::getInstance()->put(symbol::Symbol(_szName), *pVar);
}

static void Add_Double_Constant(wstring _szName, double _dblReal, double _dblImg, bool _bComplex)
{
    types::Double * pVal = new types::Double(1, 1, _bComplex);
    pVal->set(0, 0, _dblReal);
    pVal->setImg(0, 0, _dblImg);
    symbol::Context::getInstance()->put(symbol::Symbol(_szName), *pVal);
}

static void Add_Boolean_Constant(wstring _szName, bool _bBool)
{
    types::Bool * pVal = new types::Bool(_bBool);
    symbol::Context::getInstance()->put(symbol::Symbol(_szName), *pVal);
}

static void Add_String_Constant(wstring _szName, const char *_pstString)
{
    types::String * ps = new types::String(_pstString);
    symbol::Context::getInstance()->put(symbol::Symbol(_szName), *ps);
}
