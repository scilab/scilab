/* error.c -- print out the error strings. */

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

static const char *errlst[] = {
    "Error 0",
    "cannot open file",			/* 1 DLD_ENOFILE */
    "bad magic number",			/* 2 DLD_EBADMAGIC */
    "fail to read header",		/* 3 DLD_EBADHEADER */
    "premature eof in text section",	/* 4 DLD_ENOTEXT */
    "premature eof in symbols",		/* 5 DLD_ENOSYMBOLS */
    "bad string table",			/* 6 DLD_ENOSTRINGS */
    "premature eof in text relocation",	/* 7 DLD_ENOTXTRELOC */
    "premature eof in data section",	/* 8 DLD_ENODATA */
    "premature eof in data relocation",	/* 9 DLD_ENODATRELOC */
    "multiple definitions of symbol",	/* 10 DLD_EMULTDEFS */
    "malformed library archive",	/* 11 DLD_EBADLIBRARY */
    "common block not supported",	/* 12 DLD_EBADCOMMON */
    "malformed input file",		/* 13 DLD_EBADOBJECT */
    "bad relocation info",		/* 14 DLD_EBADRELOC */
    "virtual memory exhausted",		/* 15 DLD_ENOMEMORY */
    "undefined symbol"			/* 16 DLD_EUNDEFSYM */
    };

char dld_str_error[ 256 ];


/* Prints out the given string and the error message corresponding to
   dld_errno to the stderr. */
void
dld_perror (str)
const char *str;
{
    if (str)
	fprintf (stderr, "%s: ", str);

    if (dld_errno < 1 || dld_errno > sizeof (errlst)/sizeof (char *))
	fprintf (stderr, "Unknown error.\n");
    else if (DLD_EMULTDEFS==dld_errno)
      fprintf (stderr, "%s: %s\n", errlst[dld_errno], dld_errname);
    else fprintf (stderr, "%s.\n", errlst[dld_errno]);
} /* dld_perror */

/* Returns the error message corresponding to the given error code. */
char *
dld_strerror (code)
int code;
{
    if (code < 1 || code > sizeof (errlst)/sizeof (char *))
	strcpy (dld_str_error, "Unknown error");
    else strcpy (dld_str_error, errlst[code]);
    return (dld_str_error);
} /* dld_perror */
