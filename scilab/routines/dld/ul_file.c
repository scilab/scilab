/* unlink_file.c -- unlink a given object file. */

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

static struct file_entry *
search_files (entry, name)
register struct file_entry *entry;
register const char *name;
{
    while (entry) {
	if (entry->library_flag) {
	    register struct file_entry *subentry;

	    if ((subentry = search_files (entry->subfiles, name)) != 0)
		return subentry;
	} else {
	    if (!strcmp (entry->local_sym_name, name))
		return entry;
	    if (entry->local_sym_name != entry->filename)
		if (!strcmp (entry->filename, name))
		    return entry;
	}

	entry = entry->chain;
    }
    return 0;
} /* search_files */

int
dld_unlink_by_file (name, force)
const char *name;
int force;
{
    register struct file_entry *entry;

    if ((entry = search_files (_dld_latest_entry, name))) {
	_dld_unlink_entry (entry, force);
	return 0;
    }

    dld_errno = DLD_EUNDEFSYM;
    return dld_errno;
} /* dld_unlink_by_file */


