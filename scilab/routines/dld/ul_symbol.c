/* unlink_symbol.c -- unlink the object file defining the given symbol. */

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
#if defined (linux)	 
#include <string.h>
#endif


int
dld_unlink_by_symbol (name, force)
const char *name;
int force;
{
    register symbol *sp;
    register char *p;

    if (name == 0) {
	dld_errno = DLD_EUNDEFSYM;
	return dld_errno;
    }

    if (setjmp (_dld_env))
	return dld_errno;

    /* prepend a '_' to name, as required by C's convention */
    p = (char *) _dld_malloc (strlen(name) + 2);
    *p = '_';
    strcpy (p+1, name);

    sp = _dld_getsym_soft (p);
    free (p);

    if (sp && sp->defined_by) {
	if (sp->defined_by != _dld_dummy_entry)
	    _dld_unlink_entry (sp->defined_by, force);
	else dld_remove_defined_symbol (name);

	return 0;
    }

    dld_errno = DLD_EUNDEFSYM;
    return dld_errno;
} /* dld_unlink_by_symbol */


