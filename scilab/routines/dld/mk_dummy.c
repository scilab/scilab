/* mk_dummy.c -- create a dummy file_entry for definitions and
   declarations of symbols explicitly make by the user. */

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

void
_dld_create_dummy_entry () {

    register struct file_entry *entry;

    if (_dld_dummy_entry)
	return;

    entry = (struct file_entry *) _dld_malloc (sizeof (struct file_entry));

    bzero (entry, sizeof (struct file_entry));
    entry->filename = entry->local_sym_name = "-*- dummy entry -*-";
    entry->chain = _dld_latest_entry;
    _dld_latest_entry = entry;
    entry->ref_count = 1;
    _dld_dummy_entry = entry;

    return;
} /* _dld_create_dummy_entry */
