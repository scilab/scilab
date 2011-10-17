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

#include		<termios.h>
#include		<curses.h>
#include		<term.h>
#include		<unistd.h>
#include		<stdlib.h>
#include		<stdio.h>
#include		<locale.h>
#include		"init_tc_shell.h"

void canonicMode(struct termios *t)
{
    t->c_lflag |= ICANON;
    t->c_lflag |= ECHO;
}

void rawMode(struct termios *t)
{
    t->c_lflag &= ~ICANON;
    t->c_lflag &= ~ECHO;
    t->c_cc[VMIN] = 1;
    t->c_cc[VTIME] = 0;
}

/* Set Raw mode or Canonic Mode */
int setAttr(int bin)
{
    struct termios t;

    if (tcgetattr(0, &t) == -1)
    {
        fprintf(stderr, "Cannot access to the term attributes.");
        return -1;
    }
    if (bin == CANON)
        canonicMode(&t);
    else if (bin == RAW)
        rawMode(&t);
    if (tcsetattr(0, 0, &t) == -1)
    {
        fprintf(stderr, "Cannot change the term attributes.");
        return -1;
    }
    return 0;
}

/* Initialise console mode */
int initConsoleMode(int bin)
{
    if (tgetent(NULL, getenv("TERM")) == ERR && tgetent(NULL, "xterm") == ERR)
/* What about kfreebsd, bsd, mac os ? */
    {
        fprintf(stderr, "Cannot init termcaps");
        return -1;
    }
    if (setlocale(LC_CTYPE, "") == NULL)
    {
/* Please use fprintf(stderr) */
        fprintf(stderr, "Cannot set wide char.");
        return -1;
    }

    return setAttr(bin);
}
