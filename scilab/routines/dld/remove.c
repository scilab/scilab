/* remove.c -- remove an explicitly defined symbol. */

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
/*
 *  remove a previously defined symbol defined by dld_define().
 *  If symbol is not defined by dld_fine, this function becomes no-op.
 */
void
dld_remove_defined_symbol (name)
const char *name;
{
    register symbol *sp;
    register char *p;

    if (name == 0)
	return;

    p = (char *) _dld_malloc (strlen(name) +2);
    *p = '_';
    strcpy (p+1, name);

    sp = _dld_getsym_soft (p);
    free (p);

    if (sp->defined_by && sp->defined_by == _dld_dummy_entry) {
	register struct file_chain *p = sp->referenced_by;
	register struct file_chain *prev = 0;

	while (p)
	    if (p->entry->ref_count == 0) {
		del_link_list_elt (sp->referenced_by, prev, p, next);
	    } else {
		p->entry->undefined_symbol_count++;
		prev = p;
		p = p->next;
	    }

	if (sp->referenced_by) {
	    dld_undefined_sym_count++;
	    sp->value = 0;
	}

#ifndef linux
	if (sp->defined == (N_COMM | N_EXT) && sp->value) {
#else
	if (sp->defined == (N_TYPE | N_EXT) && sp->value) {
#endif
	    free ((void *)sp->value);
	    sp->value = 0;
	}

	sp->defined = 0;
	sp->defined_by = 0;

	if (sp->referenced_by) {
	    _dld_patch_all_files (_dld_latest_entry);
	    _dld_exec_flags_valid = 0;
	}
    }

} /* dld_remove_defined_symbol */
