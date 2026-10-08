/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
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

#ifndef __CONFIGVARIABLE_INTERFACE_H__
#define __CONFIGVARIABLE_INTERFACE_H__

#include "dynlib_ast.h"
#include <string.h>
#include <wchar.h>

#define VERBOSE_ERROR 0

/**
 * Describe the current Scilab features available ; output value of `getScilabMode()`
 */
typedef enum
{
    SCILAB_STD = 2, /* The standard Scilab (gui, plot ...) */
    SCILAB_NW,      /* Scilab in command line with the plots */
    SCILAB_NWNI     /* Scilab in command line without any graphics. What not mandataroy here */
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
EXTERN_AST int isSilentError(void);
EXTERN_AST void setSilentError(int _iSilent);
EXTERN_AST int isPrintInput();
EXTERN_AST void setPrintInput(int);
EXTERN_AST int isPrintOutput();
EXTERN_AST void setPrintOutput(int);
EXTERN_AST int isPrintCompact();
EXTERN_AST void setPrintCompact(int);
EXTERN_AST int isPrintInteractive();
EXTERN_AST void setPrintInteractive(int);

EXTERN_AST int getieee(void);
EXTERN_AST void setieee(int);

EXTERN_AST scilabMode getScilabMode(void);
EXTERN_AST void setScilabMode(scilabMode newmode);
EXTERN_AST const char* getScilabModeString(void);

EXTERN_AST int getWarningMode(void);
EXTERN_AST void setWarningMode(int _iMode);
EXTERN_AST int getWarningStop(void);
EXTERN_AST void setWarningStop(int _iMode);

EXTERN_AST int checkReferenceModule(const wchar_t* _module);
EXTERN_AST void addReferenceModule(const wchar_t* _module);
EXTERN_AST void removeReferenceModule(const wchar_t* _module);

EXTERN_AST int getForceQuit();
EXTERN_AST void setForceQuit(int force);

EXTERN_AST int getEntryPointPosition(wchar_t* _pwstEntryPointName);
typedef void (*dynlib_ptr)();
EXTERN_AST dynlib_ptr getEntryPointFromPosition(int position);

EXTERN_AST int getPauseLevel(void);

/* Whether a debugged execution is going on, running or stopped. */
EXTERN_AST int isDebugMode();

/*
 * True only when the console is itself the debugger interface (debug()).
 * An external debugger such as DAP leaves the console alone.
 */
EXTERN_AST int isDebugConsoleMode();
EXTERN_AST int isDebugInterrupted();

/*
 * What a user interface shows of the debugger: 0 outside debug mode, 1 in
 * debug mode with nothing running, 2 while a debugged execution runs, 3 while
 * it is stopped.
 */
EXTERN_AST int getDebugState();
EXTERN_AST int debuggerManagerExecute(const char* command, int iWaitForIt);

EXTERN_AST int isExecutionBreak();
EXTERN_AST void setExecutionBreak();
EXTERN_AST void resetExecutionBreak();

EXTERN_AST int setRecursionLimit(int);

EXTERN_AST void setPolynomialDisplay(int);
EXTERN_AST int getPolynomialDisplay();

/** Scilab started from call_scilab or javasci */
EXTERN_AST int isAPIMode();
EXTERN_AST void setAPIMode();

EXTERN_AST int startSwingView();

#endif /* !__CONFIGVARIABLE_INTERFACE_H__ */
