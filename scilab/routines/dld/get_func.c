/* get_func.c -- return the entry point of the given function. */

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

/*  given a function name, return the location of that function (in core) */
unsigned long
dld_get_func (name)
const char name[];
{
    register symbol *sp;
    register char *p;

    if (name == 0)
	return 0;

    if (setjmp (_dld_env))
	return 0;

    /* prepend a '_' to name, as required by C's convention */
    p = (char *) _dld_malloc (strlen(name) + 2);
    *p = '_';
    strcpy (p+1, name);

    sp = _dld_getsym_soft (p);
    free (p);

#ifdef linux
    if (sp) {
	while (sp->indirect) sp = sp->indirect;
	if ((sp->defined == (N_EXT | N_TEXT)) || (sp->defined == (N_EXT | N_ABS)))
	    return sp->value;
    }
#else
    if (sp)
	if (sp->defined == (N_EXT | N_TEXT))
	    return sp->value;
#endif
    return 0;
} /* dld_get_func */


