/* list_undefined.c -- return a list of strings containing the names of the
   undefined symbols. */

/* This file is part of DLD, a dynamic link/unlink editor for C.

   Copyright (C) 1990 by W. Wilson Ho.

   The author can be reached electronically by how@cs.ucdavis.edu or
   through physical mail at:

   W. Wilson Ho
   Division of Computer Science
   University of California at Davis
   Davis, CA 95616
 */

/* This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 1, or (at your option) any
   later version. */

#include "defs.h"

/*
 * Return an array of strings, each of which is the name of an undefined
 * symbol.  Return 0 if there is no undefined symbol.
 *
 * The caller is responsible to free this array.
 */
char **
dld_list_undefined_sym ()
{
    register symbol *sp;
    register int i;
    register int count = 0;
    register char **list;

    if (!dld_undefined_sym_count)
	return 0;

    if (setjmp (_dld_env))
	return 0;

    list = (char **) _dld_malloc (sizeof(char *) * dld_undefined_sym_count);

    for (i=0; i<TABSIZE; i++)
	for (sp = _dld_symtab[i]; sp; sp = sp->link)
	    if (!sp->defined) {
		list[count++] = sp->name;
		if (count >= dld_undefined_sym_count)
		    break;
	    }

    return list;
} /* dld_list_undefined_sym */

