/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2006-2008 - DIGITEO - Bruno JOFRET
 *  Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#pragma comment(lib,"../../../../../bin/libintl.lib")

#include <cstdio>
#include <iostream>
#include <string.h>

extern "C"
{
#ifdef __APPLE__
#include "initMacOSXEnv.h"
#endif
#include "InitScilab.h"
#include "sci_mode.h"
#include "scilabRead.h"
#include "ConsoleRead.h"

    extern char *getCmdLine(void);
}

#include "configvariable.hxx"
#include "exit_status.hxx"
#include "scilabWrite.hxx"

#define INTERACTIVE     -1

/*
** Usage
**
** Display usage : options available in YaSp
*/
static void usage(void)
{
    std::cerr << "Usage: Scilab <options>" << std::endl;
    std::cerr << "      -f file          : Batch mode on the given file." << std::endl;
    std::cerr << "      -l lang          : Change the language of scilab ( default : en_US )." << std::endl;
    std::cerr << "      -nw              : Enable console mode." << std::endl;
    std::cerr << "      -nwni            : Enable terminal mode." << std::endl;
    std::cerr << "      -ns              : Don't execute etc/scilab.start." << std::endl;
    std::cerr << "      --help           : Display this help." << std::endl;
    std::cerr << "Developer Trace arguments:" << std::endl;
    std::cerr << "      --parse-trace    : Display bison state machine evolution." << std::endl;
    std::cerr << "      --AST-trace      : Display ASCII-art AST to be human readable." << std::endl;
    std::cerr << "      --pretty-print   : Display pretty-printed code, standard Scilab syntax." << std::endl;
    std::cerr << " " << std::endl;
    std::cerr << "Developer Timer arguments:" << std::endl;
    std::cerr << "      --AST-timed      : Time each AST node." << std::endl;
    std::cerr << "      --timed          : Time global execution." << std::endl;
    std::cerr << " " << std::endl;
    std::cerr << "Developer Debug arguments:" << std::endl;
    std::cerr << "      --no-exec        : Only do Lexing/parsing do not execute instructions." << std::endl;
    std::cerr << "      --context-dump   : Display context status." << std::endl;
    std::cerr << "      --exec-verbose   : Display command before running it." << std::endl;
}

/*
** Get Options
**
*/
static int get_option(const int argc, char *argv[], ScilabEngineInfo* _pSEI)
{
    int i = 0;

#ifdef DEBUG
    std::cerr << "-*- Getting Options -*-" << std::endl;
#endif

    for (i = 1; i < argc; ++i)
    {
        if (!strcmp("--parse-trace", argv[i]))
        {
            _pSEI->iParseTrace = 1;
        }
        else if (!strcmp("--pretty-print", argv[i]))
        {
            _pSEI->iPrintAst = 1;
        }
        else if (!strcmp("--help", argv[i]))
        {
            usage();
            exit(WELL_DONE);
        }
        else if (!strcmp("--AST-trace", argv[i]))
        {
            _pSEI->iDumpAst = 1;
        }
        else if (!strcmp("--no-exec", argv[i]))
        {
            _pSEI->iExecAst = 0;
        }
        else if (!strcmp("--context-dump", argv[i]))
        {
            _pSEI->iDumpStack = 1;
        }
        else if (!strcmp("--timed", argv[i]))
        {
            _pSEI->iTimed = 1;
        }
        else if (!strcmp("--AST-timed", argv[i]))
        {
            std::cout << "Timed execution" << std::endl;
            _pSEI->iAstTimed = 1;
        }
        else if (!strcmp("--parse-file", argv[i]))
        {
            i++;
            if (argc >= i)
            {
                _pSEI->pstParseFile = argv[i];
            }
        }
        else if (!strcmp("-f", argv[i]))
        {
            i++;
            if (argc >= i)
            {
                _pSEI->pstFile = argv[i];
            }
        }
        else if (!strcmp("-e", argv[i]))
        {
            i++;
            if (argc >= i)
            {
                _pSEI->pstExec = argv[i];
            }
        }
        else if (!strcmp("-l", argv[i]))
        {
            i++;
            if (argc >= i)
            {
                _pSEI->pstLang = argv[i];
            }
        }
        else if (!strcmp("-nw", argv[i]))
        {
            _pSEI->iConsoleMode = 1;
        }
        else if (!strcmp("-nwni", argv[i]))
        {
            _pSEI->iConsoleMode = 1;
            _pSEI->iNoJvm = 1;
        }
        else if (!strcmp("-ns", argv[i]))
        {
            _pSEI->iNoStart = 1;
        }
        else if (!strcmp("--exec-verbose", argv[i]))
        {
            _pSEI->iExecVerbose = 1;
        }
    }

#ifdef DEBUG
    std::cerr << "File : " << argv[*_piFileIndex] << std::endl;
#endif

    ConfigVariable::setCommandLineArgs(argc, argv);
    return 0;
}

extern "C"
{
#include <stdio.h>

#ifndef _MSC_VER
#include <unistd.h>
#endif
    extern char *getCmdLine(void);
    extern void ConsolePrintf(char *);
}

static void TermPrintf(char *text)
{
    //std::cout << text;
    printf("%s", text);
}

/*
** -*- MAIN -*-
*/
//#if defined(_WIN32) && !defined(WITHOUT_GUI)
//int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
//#else
int main(int argc, char *argv[])
//#endif
{
    int iRet = 0;
    ScilabEngineInfo* pSEI = InitScilabEngineInfo();
#ifdef WITHOUT_GUI
    /* Building Scilab-cli-bin. We won't ever had the gui nor the jvm */
    pSEI->iConsoleMode = 1;
    pSEI->iNoJvm = 1;
    setScilabMode(SCILAB_NWNI);
#else
    setScilabMode(SCILAB_STD);
#endif

    //#if defined(_WIN32) && !defined(WITHOUT_GUI)
    //    {
    //        LPSTR my_argv[256];
    //        int nArgs = 0;
    //        LPWSTR *szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
    //        if (szArglist)
    //        {
    //            for (int i = 0; i < nArgs; i++)
    //            {
    //                my_argv[i] = wide_string_to_UTF8(szArglist[i]);
    //            }
    //            LocalFree(szArglist);
    //        }
    //        get_option(nArgs, my_argv, pSEI);
    //    }
    //#else
    get_option(argc, argv, pSEI);
    //#endif

    // if WITHOUT_GUI is defined
    // force Terminal IO -> Terminal IO + StartScilabEngine

    // WITHOUT_GUI (All Platform) => Terminal IO + StartScilabEngine
    // GUI (MacOSX) =>      [no option]     -> Console IO + InitMacOSXEnv
    //                      | [-nwni]       -> Terminal IO + StartScilabEngine
    //                      | [-nw]         -> Terminal IO + InitMacOSXEnv
#ifndef WITHOUT_GUI
    if (pSEI->iConsoleMode)
    {
        setScilabMode(SCILAB_NW);
        setScilabInputMethod(&getCmdLine);
        setScilabOutputMethod(&TermPrintf);
#if defined(__APPLE__)
        if (pSEI->iNoJvm == 0)
        {
            return initMacOSXEnv(pSEI);
        }
#endif // !defined(__APPLE__)
    }
    else
    {
        setScilabMode(SCILAB_STD);
        setScilabInputMethod(&ConsoleRead);
        setScilabOutputMethod(&ConsolePrintf);
#if defined(__APPLE__)
        return initMacOSXEnv(pSEI);
#endif // !defined(__APPLE__)
    }
#else
    setScilabMode(SCILAB_NWNI);
    setScilabInputMethod(&getCmdLine);
    setScilabOutputMethod(&TermPrintf);
#endif // defined(WITHOUT_GUI)

    StartScilabEngine(pSEI);
    iRet = RunScilabEngine(pSEI);
    StopScilabEngine(pSEI);
    return iRet;
}
