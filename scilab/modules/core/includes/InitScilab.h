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
    enum command_origin_t iCommandOrigin;
} ScilabEngineInfo;

ScilabEngineInfo* InitScilabEngineInfo();

int StartScilabEngine(ScilabEngineInfo* _pSEI);
int RunScilabEngine(ScilabEngineInfo* _pSEI);
int ExecExternalCommand(ScilabEngineInfo* _pSEI);

void StopScilabEngine(ScilabEngineInfo* _pSEI);

#endif /* !__INITSCILAB_H__ */
