/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - DIGITEO - Allan CORNET
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
/*--------------------------------------------------------------------------*/
#include <Windows.h>
#include <stdio.h>
#include <ctype.h>
#include "TermConsole.h"
#include "sci_malloc.h"
#include "TermCommand.h"
#include "FocusOnConsole.h"
#include "os_string.h"
#include "TermCompletion.h"
#include "TermLine.h"
#include "scilines.h"
#include "HistoryManager.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
#ifdef CR_1
#undef CR_1
#endif
#define CR_1 '\n'

#ifdef CR_2
#undef CR_2
#endif
#define CR_2 '\r'
/*--------------------------------------------------------------------------*/
static HANDLE Win32OutputStream = NULL, Win32InputStream = NULL;
static DWORD OldWin32Mode;
/*--------------------------------------------------------------------------*/
static BOOL InitTerm = TRUE;
/*--------------------------------------------------------------------------*/
static unsigned char TerminalGetchar(void);
static BOOL isCTRLPressed(DWORD StateKey);
static BOOL isCTRL_VKEY(int VKEY);
static BOOL isALTPressed(DWORD StateKey);
static BOOL isALT_VKEY(int VKEY);
static BOOL isExtendedPressed(DWORD StateKey);
static void simulateCarriageReturn(void);
static char actionControlKey(void);
/*--------------------------------------------------------------------------*/
static BOOL CtrlHandler( DWORD fdwCtrlType )
{
    switch ( fdwCtrlType )
    {
        case CTRL_C_EVENT:
        {
            ControlC_Command();
            newLine();
            simulateCarriageReturn();
        }
        return TRUE;
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
static void simulateCarriageReturn(void)
{
    INPUT_RECORD rec;
    DWORD written;

    memset (&rec, 0, sizeof(rec));
    rec.EventType = KEY_EVENT;
    rec.Event.KeyEvent.bKeyDown = TRUE;
    rec.Event.KeyEvent.wRepeatCount = 13;
    rec.Event.KeyEvent.uChar.AsciiChar = 13;

    if (!Win32InputStream)
    {
        Win32InputStream = GetStdHandle(STD_INPUT_HANDLE);
    }
    WriteConsoleInput(Win32InputStream, &rec, 1, &written);
}
/*--------------------------------------------------------------------------*/
void InitializeTerminal(void)
{
    if (!Win32InputStream)
    {
        Win32InputStream = GetStdHandle(STD_INPUT_HANDLE);
        GetConsoleMode(Win32InputStream, &OldWin32Mode);
        SetConsoleMode(Win32InputStream, ENABLE_PROCESSED_INPUT);
    }

    if (!Win32OutputStream)
    {
        Win32OutputStream = GetStdHandle(STD_OUTPUT_HANDLE);
    }

    setFocusOnConsole();

    SetConsoleCtrlHandler( (PHANDLER_ROUTINE) CtrlHandler, TRUE );

}
/*--------------------------------------------------------------------------*/
void TerminalBeep(void)
{
    MessageBeep(MB_OK);
}
/*--------------------------------------------------------------------------*/
int TerminalPrintf(char *buffer)
{
    if (buffer)
    {
        if (buffer[0] != 0)
        {
            int len = (int)strlen (buffer);
            /* flush all stream */
            /* problem with fortran output */
            fflush(NULL);

            len = fputs (buffer, stdout);

            /* flush all stream */
            /* problem with fortran output */
            fflush(NULL);

            return len;
        }
        return 0;
    }
    return -1;
}
/*--------------------------------------------------------------------------*/
int TerminalPutc(char ch)
{
    return putc(ch, stdout);
}
/*--------------------------------------------------------------------------*/
static unsigned char TerminalGetchar(void)
{
    INPUT_RECORD irBuffer;
    DWORD n = 0;
    unsigned char ch = 0;
    do
    {
        /* http://bugzilla.scilab.org/show_bug.cgi?id=1052 */
        if ( ismenu() == 1 )
        {
            return 0;
        }

        WaitForSingleObject(Win32InputStream, INFINITE);
        PeekConsoleInput (Win32InputStream, &irBuffer, 1, &n);

        switch (irBuffer.EventType)
        {
            case KEY_EVENT:
            {
                if (irBuffer.Event.KeyEvent.bKeyDown)
                {
                    if (irBuffer.Event.KeyEvent.dwControlKeyState)
                    {
                        if (isCTRLPressed(irBuffer.Event.KeyEvent.dwControlKeyState))
                        {
                            char c = actionControlKey();
                            if (c)
                            {
                                ReadConsoleInputW (Win32InputStream, &irBuffer, 1, &n);
                                return c;
                            }
                            else
                            {
                                if (irBuffer.Event.KeyEvent.uChar.AsciiChar != '\0')
                                {
                                    ReadConsoleInputW (Win32InputStream, &irBuffer, 1, &n);
                                    c = irBuffer.Event.KeyEvent.uChar.AsciiChar;
                                    if ( (c > 0) && !iscntrl(c) )
                                    {
                                        return c;
                                    }
                                }
                                else
                                {
                                    ReadConsoleInput (Win32InputStream, &irBuffer, 1, &n);
                                }
                            }
                            break;
                        }

                        if (isALTPressed(irBuffer.Event.KeyEvent.dwControlKeyState))
                        {
                            if (irBuffer.Event.KeyEvent.uChar.AsciiChar != '\0')
                            {
                                ReadConsole (Win32InputStream, &ch, 1, &n, NULL);
                                return ch;
                            }
                            else
                            {
                                DWORD stateKey = 0;
                                WORD vk = 0;

                                ReadConsoleInput (Win32InputStream, &irBuffer, 1, &n);

                                stateKey = irBuffer.Event.KeyEvent.dwControlKeyState;
                                vk = irBuffer.Event.KeyEvent.wVirtualKeyCode;

                                switch (vk)
                                {
                                    case VK_F4:
                                        ALTF4_Command();
                                        break;

                                    default:
                                        break;
                                }
                            }
                            break;
                        }
                    }

                    if (irBuffer.Event.KeyEvent.uChar.AsciiChar != '\0')
                    {
                        ReadConsole (Win32InputStream, &ch, 1, &n, NULL);

                        switch (ch)
                        {
                            case VK_TAB:
                                TermCompletion();
                                break;
                            case VK_BACK:
                                deletePreviousChar();
                                break;
                            default:
                            {
                                if ( !iscntrl(ch) || (ch == CR_1) || (ch == CR_2) )
                                {
                                    return ch;
                                }
                            }
                            break;
                        }
                    }
                    else
                    {
                        WORD vk = 0;
                        ReadConsoleInput (Win32InputStream, &irBuffer, 1, &n);
                        vk = irBuffer.Event.KeyEvent.wVirtualKeyCode;

                        switch (vk)
                        {
                            case VK_F1:
                            case VK_HELP:
                                F1_Command();
                                break;
                            case VK_F2:
                                F2_Command();
                                break;
                            case VK_LEFT:
                                moveBackSingleChar();
                                break;
                            case VK_RIGHT:
                                moveForwardSingleChar();
                                break;
                            case VK_UP:
                                moveBackHistory();
                                break;
                            case VK_DOWN:
                                moveForwardHistory();
                                break;
                            case VK_DELETE:
                                deleteCurrentChar();
                                break;
                            case VK_HOME:
                                moveBeginningLine();
                                break;
                            case VK_END:
                                moveEndLine();
                                break;
                            default:
                                break;
                        }
                    }
                }
                else
                {
                    ReadConsoleInput (Win32InputStream, &irBuffer, 1, &n);
                }
            }
            break;
            case MOUSE_EVENT:
            {
                /* Read mouse Input but not used */
                ReadConsoleInput (Win32InputStream, &irBuffer, 1, &n);
            }
            break;
            case WINDOW_BUFFER_SIZE_EVENT:
            {
                /* Read resize event Input */
                setConsoleWidth(irBuffer.Event.WindowBufferSizeEvent.dwSize.X);
                setConsoleLines(irBuffer.Event.WindowBufferSizeEvent.dwSize.Y);

                ReadConsoleInput (Win32InputStream, &irBuffer, 1, &n);
            }
            break;
            case MENU_EVENT:
            {
                ReadConsoleInput (Win32InputStream, &irBuffer, 1, &n);
            }
            break;
            case FOCUS_EVENT:
            {
                ReadConsoleInput (Win32InputStream, &irBuffer, 1, &n);
            }
            break;
            default:
            {
                /* Read Input but not used */
                ReadConsoleInput (Win32InputStream, &irBuffer, 1, &n);
            }
            break;
        }
    }
    while (TRUE);
}
/*--------------------------------------------------------------------------*/
static char actionControlKey(void)
{
    if ( isCTRL_VKEY('X') || isCTRL_VKEY('C') )
    {
        ControlC_Command();
        return '\n';
    }
    else if (isCTRL_VKEY('A')) /* moves to the beginning of the line */
    {
        moveBeginningLine();
    }
    else if (isCTRL_VKEY('B')) /* moves back a single character */
    {
        moveBackSingleChar();
    }
    else if (isCTRL_VKEY('D')) /* deletes the current character */
    {
        deleteCurrentChar();
    }
    else if (isCTRL_VKEY('E')) /* moves to the end of the line */
    {
        moveEndLine();
    }
    else if (isCTRL_VKEY('F')) /* moves forward a single character */
    {
        moveForwardSingleChar();
    }
    else if (isCTRL_VKEY('H')) /* delete the previous character */
    {
        deletePreviousChar();
    }
    else if (isCTRL_VKEY('K')) /* kills from current position to the end of line */
    {
        killCurrentPositionToEndLine();
    }
    else if (isCTRL_VKEY('N')) /* moves forward through history */
    {
        moveForwardHistory();
    }
    else if (isCTRL_VKEY('P')) /* moves back through history */
    {
        moveBackHistory();
    }
    else if ( isCTRL_VKEY('R') || isCTRL_VKEY('L') ) /* redraw line in case it gets trashed */
    {
        redrawLine();
    }
    else if (isCTRL_VKEY('U')) /* kills the entire line */
    {
        clearCurrentLine();
    }
    else if (isCTRL_VKEY('V'))
    {
        pasteClipBoard();
    }
    else if (isCTRL_VKEY('W')) /* kills last word */
    {
        killLastWord();
    }
    else if (isCTRL_VKEY(VK_TAB) || isCTRL_VKEY(VK_SPACE)) /* Completion */
    {
        TermCompletion();
    }
    else if (isCTRL_VKEY(VK_LEFT)) /* */
    {
        moveBackSingleWord();
    }
    else if (isCTRL_VKEY(VK_RIGHT)) /* */
    {
        moveForwardSingleWord();
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
char *TerminalGetString(char *prompt)
{
    if (InitTerm)
    {
        InitializeTerminal();
        InitTerm = FALSE;
    }

    newLine();

    setCurrentPrompt(prompt);

    /* print the prompt */
    displayPrompt();

    /* initialize history search */
    setSearchedTokenInScilabHistory(NULL);

    for (;;)
    {
        unsigned char cur_char = TerminalGetchar();

        if (cur_char <= 0)
        {
            return NULL;
        }

        /* http://bugzilla.scilab.org/show_bug.cgi?id=1052 */
        if (ismenu () == 1)
        {
            /* Abort current line */
            return NULL;
        }

        if ( (cur_char == CR_1) || (cur_char == CR_2) )
        {
            if ( isHistorySearch() )
            {
                putLineSearchedHistory();
            }
            else
            {
                char *line = getCurrentLine();
                TerminalPutc('\n');
                appendLineToScilabHistory(line);
                return line;
            }
        }
        else
        {
            TerminalPutc(cur_char);
            addCharacterCurrentLine(cur_char);
        }
    }
    return NULL;
}
/*--------------------------------------------------------------------------*/
static BOOL isCTRLPressed(DWORD StateKey)
{
    return ((StateKey & (RIGHT_CTRL_PRESSED | LEFT_CTRL_PRESSED)) != 0);
}
/*--------------------------------------------------------------------------*/
static BOOL isALTPressed(DWORD StateKey)
{
    return ((StateKey & (RIGHT_ALT_PRESSED | LEFT_ALT_PRESSED)) != 0);
}
/*--------------------------------------------------------------------------*/
static BOOL isExtendedPressed(DWORD StateKey)
{
    return ((StateKey & (ENHANCED_KEY)) != 0);
}
/*--------------------------------------------------------------------------*/
static BOOL isCTRL_VKEY(int VKEY)
{
    return ( GetKeyState(VKEY) & 0x80 );
}
/*--------------------------------------------------------------------------*/
