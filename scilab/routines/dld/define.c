/* define.c -- explicitly define a named symbol. */

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
 *  explicitly define the value for symbol NAME.
 *
 *  SIZE is the number of bytes required.
 */
int
dld_define_sym (name, size)
const char *name;
unsigned int size;
{
    register char *p = 0;
    struct nlist dummy_nlist;	    /* simulate a nlist entry so that
				       _dld_entry_global_ref can be used. */
    register int old_undefined_sym_count;

    if (name == 0 || size == 0)
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
    dummy_nlist.n_type = N_UNDF | N_EXT;
    dummy_nlist.n_value = size;

    old_undefined_sym_count = dld_undefined_sym_count;
    _dld_enter_global_ref (_dld_dummy_entry, &dummy_nlist, p);

    free (p);

    if (old_undefined_sym_count != dld_undefined_sym_count) {
	_dld_patch_all_files (_dld_latest_entry);
	_dld_exec_flags_valid = 0;
    }

    return 0;
} /* dld_define_sym */
