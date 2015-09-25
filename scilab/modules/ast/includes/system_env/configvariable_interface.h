/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __CONFIGVARIABLE_INTERFACE_H__
#define __CONFIGVARIABLE_INTERFACE_H__

#include <string.h>
#include <wchar.h>
#include "dynlib_ast.h"

#define PROMPTMODE_NORMAL   0   //show new values but not commands
#define PROMPTMODE_SILENT   -1  //hide all
#define PROMPTMODE_PROMPT   2   //show all
#define PROMPTMODE_EXEC     1   //show all
#define PROMPTMODE_EXEC3    3   //show all
#define PROMPTMODE_STEP     4   //later ...
#define PROMPTMODE_STEP7    7   //later ...

#define SILENT_ERROR        1
#define VERBOSE_ERROR       0

typedef enum
{
    SCILAB_API = 1, /* Scilab is launch as an API */
    SCILAB_STD, /* The standard Scilab (gui, plot ...) */
    SCILAB_NW, /* Scilab in command line with the plots */
    SCILAB_NWNI /* Scilab in command line without any graphics. What not mandataroy here */
} scilabMode;

EXTERN_AST int getSimpMode(void);
EXTERN_AST void setSimpMode(int);
EXTERN_AST int getStartProcessing();
EXTERN_AST int getEndProcessing();

EXTERN_AST void setFormatSize(int _iFormatSize);
EXTERN_AST int getFormatSize(void);
EXTERN_AST void setFormatMode(int _iFormatMode);
EXTERN_AST int getFormatMode(void);
EXTERN_AST void setConsoleWidth(int _iConsoleWidth);
EXTERN_AST int getConsoleWidth(void);
EXTERN_AST void setConsoleLines(int _iConsoleLines);
EXTERN_AST int getConsoleLines(void);

EXTERN_AST int getPromptMode(void);
EXTERN_AST void setPromptMode(int _iMode);
EXTERN_AST int isPromptShow(void);
EXTERN_AST int getSilentError(void);
EXTERN_AST void setSilentError(int _iSilent);

EXTERN_AST int getieee(void);
EXTERN_AST void setieee(int);

EXTERN_AST scilabMode getScilabMode(void);
EXTERN_AST void setScilabMode(scilabMode newmode);
EXTERN_AST const char * getScilabModeString(void);

EXTERN_AST int getWarningMode(void);
EXTERN_AST void setWarningMode(int _iMode);

EXTERN_AST int checkReferenceModule(const wchar_t* _module);
EXTERN_AST void addReferenceModule(const wchar_t* _module);
EXTERN_AST void removeReferenceModule(const wchar_t* _module);

EXTERN_AST int getForceQuit();

EXTERN_AST int getEntryPointPosition(wchar_t* _pwstEntryPointName);
typedef void(*dynlib_ptr)();
EXTERN_AST dynlib_ptr getEntryPointFromPosition(int position);

EXTERN_AST int isEnableDebug();
EXTERN_AST int isDebugInterrupted();

EXTERN_AST int isExecutionBreak();
EXTERN_AST void setExecutionBreak();
EXTERN_AST void resetExecutionBreak();

EXTERN_AST int setRecursionLimit(int);
#endif /* !__CONFIGVARIABLE_INTERFACE_H__ */
