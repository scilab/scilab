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

#ifndef __INITSCILAB_H__
#define __INITSCILAB_H__

#include "storeCommand.h" // command_origin_t

typedef struct
{
    char* pstParseFile;
    char* pstFile;
    char* pstExec;
    char* pstLang;
    void* pExpTree;
    int iParseTrace;
    int iPrintAst;
    int iExecAst;
    int iDumpAst;
    int iDumpStack;
    int iTimed;
    int iAstTimed;
    int iExecVerbose;
    int iConsoleMode;
    int iNoJvm;
    int iNoStart;
    int iShowVersion;
    int iSerialize;
    int iKeepConsole;
    int iNoBanner;
    int iMultiLine;
    int isInterruptible;
    int isPrioritary;
    int iStartConsoleThread;
    int iForceQuit;
    int iTimeoutDelay;
    int iCodeAction;
    enum command_origin_t iCommandOrigin;
} ScilabEngineInfo;

ScilabEngineInfo* InitScilabEngineInfo();

int StartScilabEngine(ScilabEngineInfo* _pSEI);
int RunScilabEngine(ScilabEngineInfo* _pSEI);
int ExecExternalCommand(ScilabEngineInfo* _pSEI);

void StopScilabEngine(ScilabEngineInfo* _pSEI);

#endif /* !__INITSCILAB_H__ */
