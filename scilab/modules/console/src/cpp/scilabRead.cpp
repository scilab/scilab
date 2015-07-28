/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "configvariable.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "scilabRead.h"
#include "SetConsolePrompt.h"
#include "prompt.h"
#include "TermReadAndProcess.h"
#include "os_string.h"
#include "configvariable_interface.h"
#include "localization.h"
#include "diary.h"
}

static SCILAB_INPUT_METHOD _reader;

void setScilabInputMethod(SCILAB_INPUT_METHOD reader)
{
    _reader = reader;
}

void C2F(scilabread)(char* strRead, int len)
{
    char* str = scilabRead();
    int size = Min(static_cast<int>(strlen(str)), len - 1);
    strncpy(strRead, str, size);
    strRead[size] = '\0';
    FREE(str);
}

char *scilabRead()
{
    if (getScilabMode() == SCILAB_STD)
    {
        /* Send new prompt to Java Console, do not display it */
        if (GetTemporaryPrompt() != NULL)
        {
            SetConsolePrompt(GetTemporaryPrompt());
        }
        else
        {
            char pstCurrentPrompt[PROMPT_SIZE_MAX];
            GetCurrentPrompt(pstCurrentPrompt);
            SetConsolePrompt(pstCurrentPrompt);
        }
    }

    //call reader
    char* pstTemp = (*_reader)();

    //add prompt to diary
    static char pstPrompt[PROMPT_SIZE_MAX];
    GetCurrentPrompt(pstPrompt);
    wchar_t* pwstPrompt = to_wide_string(pstPrompt);
    diaryWrite(pwstPrompt, TRUE);
    FREE(pwstPrompt);

    //add input to diary
    wchar_t* pwstIn = to_wide_string(pstTemp);
    diaryWriteln(pwstIn, TRUE);
    FREE(pwstIn);

    return pstTemp;
}
