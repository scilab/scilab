/* get_symbol.c -- return the address of the given symbol. */

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


/* given a symbol name, return the location of that symbol (in core) */
unsigned long
dld_get_symbol (name)
const char name[];
{
    register char *p;
    register unsigned long value;

    if (name == 0)
	return 0;

    if (setjmp (_dld_env))
	return 0;

    /* prepend a '_' to name, as required by C's convention */
    p = (char *) _dld_malloc (strlen(name) + 2);
    *p = '_';
    strcpy (p+1, name);

    value = dld_get_bare_symbol (p);
    free (p);
    return value;
} /* dld_get_symbol */
