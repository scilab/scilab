/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Karim Mamode
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*/

#include <termios.h>
#include <curses.h>
#include <term.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <errno.h>
#include "MALLOC.h"
#include "init_tc_shell.h"

static void canonicMode(struct termios *t)
{
    t->c_lflag |= ICANON;
    t->c_lflag |= ECHO;
}

static void rawMode(struct termios *t)
{
    t->c_lflag &= ~ICANON;
    t->c_lflag &= ~ECHO;
    t->c_cc[VMIN] = 1;          /* TODO: comment */
    t->c_cc[VTIME] = 0;         /* TODO: comment */
}

/* Save Shell Attribute To reset it when exit */
static void saveAndResetShellAttr(struct termios *shellAttr)
{
    static struct termios *savedAttr = NULL;

    if (savedAttr == NULL)
    {
        savedAttr = MALLOC(sizeof(struct termios));
        *savedAttr = *shellAttr;
    }
    else if (shellAttr == NULL)
    {
        if (tcsetattr(0, 0, savedAttr) == -1)
        {
            fprintf(stderr, "Cannot reset the Shell attributes: %s\n", strerror(errno));
        }
        FREE(savedAttr);
        savedAttr = NULL;
    }
}

/* Set Raw mode or Canonic Mode */
int setAttr(int bin)
{
    struct termios shellAttr;

    if (bin == ATTR_RESET)
    {
        saveAndResetShellAttr(NULL);
        return 0;
    }
    if (tcgetattr(0, &shellAttr) == -1)
    {
        fprintf(stderr, "Cannot access to the term attributes: %s\n", strerror(errno));
        return -1;
    }
    saveAndResetShellAttr(&shellAttr);
    if (bin == CANON)
    {
        canonicMode(&shellAttr);
    }
    else if (bin == RAW)
    {
        rawMode(&shellAttr);
    }
    if (tcsetattr(0, 0, &shellAttr) == -1)
    {
        fprintf(stderr, "Cannot change the term attributes: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

/* Initialise console mode */
int initConsoleMode(int bin)
{
    /* TODO: Check more term */
    if (tgetent(NULL, getenv("TERM")) == ERR && tgetent(NULL, "xterm") == ERR)
    {
        fprintf(stderr, "Cannot initialise termcaps.\nPlease check your variable TERM in your environment.\n");
        return -1;
    }
    if (setlocale(LC_CTYPE, "") == NULL)
    {
        fprintf(stderr, "Cannot set wide char.\n");
        return -1;
    }

    return setAttr(bin);
}
