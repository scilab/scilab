/* ref.c -- explicitly make a reference to a symbol so that the library
   defining this symbol is loaded. */

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
 *  explicitly create a reference to NAME so that a library routine
 *  defining it is forced to be loaded.  (c.f. "ld -u" option)
 *  It has no effect on non-library routines (plain object files).
 */
int
dld_create_reference (name)
const char *name;
{
    register char *p = 0;
    struct nlist dummy_nlist;	    /* simulate a nlist entry so that
				       _dld_entery_global_ref can be used. */

    if (name == 0)
	return 0;

    if (setjmp (_dld_env)) {
	if (p) free (p);
	return dld_errno;
    }

    if (_dld_dummy_entry == 0)
	_dld_create_dummy_entry ();

    bzero (&dummy_nlist, sizeof (struct nlist));

    p = (char *) _dld_malloc (strlen(name) +2);
    *p = '_';
    strcpy (p+1, name);

    dummy_nlist.n_un.n_name = p;
    dummy_nlist.n_type = N_EXT | N_UNDF;

    _dld_enter_global_ref (_dld_dummy_entry, &dummy_nlist, p);

    free (p);
    return 0;
} /* dld_create_reference */
