/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2006-2008 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#pragma comment(lib,"../../bin/libintl.lib")

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

static bool parseTrace = false;
static bool printAst = false;
static bool execAst = true;
static bool dumpAst = false;
static bool dumpStack = false;
static bool timed = false;
static bool ASTtimed = false;
static bool execVerbose = false;
static bool consoleMode = false;
static bool noJvm = false;
static bool noStart = false;
static bool noBanner = false;
static bool execCommand = false;
static bool execFile = false;
static bool parseFile = false;

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
static int get_option(const int argc, char *argv[], int *_piFileIndex, int *_piParseFileIndex, int *_piExecIndex, int *_piLangIndex)
{
    int i = 0;

#ifdef DEBUG
    std::cerr << "-*- Getting Options -*-" << std::endl;
#endif

    for (i = 1; i < argc; ++i)
    {
        if (!strcmp("--parse-trace", argv[i]))
        {
            parseTrace = true;
        }
        else if (!strcmp("--pretty-print", argv[i]))
        {
            printAst = true;
        }
        else if (!strcmp("--help", argv[i]))
        {
            usage();
            exit(WELL_DONE);
        }
        else if (!strcmp("--AST-trace", argv[i]))
        {
            dumpAst = true;
        }
        else if (!strcmp("--no-exec", argv[i]))
        {
            execAst = false;
        }
        else if (!strcmp("--context-dump", argv[i]))
        {
            dumpStack = true;
        }
        else if (!strcmp("--timed", argv[i]))
        {
            timed = true;
        }
        else if (!strcmp("--AST-timed", argv[i]))
        {
            std::cout << "Timed execution" << std::endl;
            ASTtimed = true;
        }
        else if (!strcmp("--parse-file", argv[i]))
        {
            i++;
            parseFile = true;
            *_piParseFileIndex = i;
        }
        else if (!strcmp("-f", argv[i]))
        {
            i++;
            execFile = true;
            *_piFileIndex = i;
        }
        else if (!strcmp("-e", argv[i]))
        {
            i++;
            execCommand = true;
            *_piExecIndex = i;
        }
        else if (!strcmp("-l", argv[i]))
        {
            i++;
            *_piLangIndex = i;
        }
        else if (!strcmp("-nw", argv[i]))
        {
            consoleMode = true;
            setScilabMode(SCILAB_NW);
        }
        else if (!strcmp("-nwni", argv[i]))
        {
            consoleMode = true;
            noJvm = true;
            setScilabMode(SCILAB_NWNI);
        }
        else if (!strcmp("-ns", argv[i]))
        {
            noStart = true;
        }
        else if (!strcmp("-nb", argv[i]))
        {
            noBanner = true;
        }
        else if (!strcmp("--exec-verbose", argv[i]))
        {
            execVerbose = true;
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
int main(int argc, char *argv[])
{
    int iRet = 0;
    int iFileIndex = 0;
    int iParseFileIndex = 0;
    int iExecIndex = 0;
    int iLangIndex = 0;

#ifdef WITHOUT_GUI
    /* Building Scilab-cli-bin. We won't ever had the gui nor the jvm */
    consoleMode = true;
    noJvm = true;
    setScilabMode(SCILAB_NWNI);
#else
    setScilabMode(SCILAB_STD);
#endif

    get_option(argc, argv, &iFileIndex, &iParseFileIndex, &iExecIndex, &iLangIndex);

    if (iFileIndex >= argc || iParseFileIndex >= argc
            || iExecIndex >= argc || iLangIndex >= argc)
    {
        // we used -l, -e or -f without another argument
        usage();
        return -1;
    }

    char* pstLang = NULL;
    char* pstFile = NULL;
    char* pstParseFile = NULL;
    char* pstExec = NULL;

    if (iLangIndex)
    {
        pstLang = argv[iLangIndex];
    }

    if (iFileIndex)
    {
        pstFile = argv[iFileIndex];
    }

    if (iParseFileIndex)
    {
        pstParseFile = argv[iParseFileIndex];
    }

    if (iExecIndex)
    {
        pstExec = argv[iExecIndex];
    }

    // if WITHOUT_GUI is defined
    // force Terminal IO -> Terminal IO + StartScilabEngine

    // WITHOUT_GUI (All Platform) => Terminal IO + StartScilabEngine
    // GUI (MacOSX) =>      [no option]     -> Console IO + InitMacOSXEnv
    //                      | [-nwni]       -> Terminal IO + StartScilabEngine
    //                      | [-nw]         -> Terminal IO + InitMacOSXEnv
#ifndef WITHOUT_GUI
    if (consoleMode)
    {
        setScilabInputMethod(&getCmdLine);
        setScilabOutputMethod(&TermPrintf);
#if defined(__APPLE__)
        if (!noJvm)
        {
            return initMacOSXEnv(argc, argv, iFileIndex);
        }
#endif // !defined(__APPLE__)
    }
    else
    {
        setScilabInputMethod(&ConsoleRead);
        setScilabOutputMethod(&ConsolePrintf);
#if defined(__APPLE__)
        return initMacOSXEnv(argc, argv, iFileIndex);
#endif // !defined(__APPLE__)
    }
#else
    setScilabInputMethod(&getCmdLine);
    setScilabOutputMethod(&TermPrintf);
#endif // defined(WITHOUT_GUI)

    StartScilabEngine(pstExec, pstFile, pstLang, noStart ? 1 : 0, noJvm ? 0 : 1, consoleMode ? 1 : 0);
    iRet = RunScilabEngine(pstParseFile);
    StopScilabEngine(NULL, noStart, consoleMode);
    return iRet;
}
