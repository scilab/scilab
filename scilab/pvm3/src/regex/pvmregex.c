
static char rcsid[] =
	"$Id: pvmregex.c,v 1.2 2002/10/14 14:38:03 chanceli Exp $";

/*
 *         PVM version 3.4:  Parallel Virtual Machine System
 *               University of Tennessee, Knoxville TN.
 *           Oak Ridge National Laboratory, Oak Ridge TN.
 *                   Emory University, Atlanta GA.
 *      Authors:  J. J. Dongarra, G. E. Fagg, M. Fischer
 *          G. A. Geist, J. A. Kohl, R. J. Manchek, P. Mucci,
 *         P. M. Papadopoulos, S. L. Scott, and V. S. Sunderam
 *                   (C) 1997 All Rights Reserved
 *
 *                              NOTICE
 *
 * Permission to use, copy, modify, and distribute this software and
 * its documentation for any purpose and without fee is hereby granted
 * provided that the above copyright notice appear in all copies and
 * that both the copyright notice and this permission notice appear in
 * supporting documentation.
 *
 * Neither the Institutions (Emory University, Oak Ridge National
 * Laboratory, and University of Tennessee) nor the Authors make any
 * representations about the suitability of this software for any
 * purpose.  This software is provided ``as is'' without express or
 * implied warranty.
 *
 * PVM version 3 was funded in part by the U.S. Department of Energy,
 * the National Science Foundation and the State of Tennessee.
 */


#include <stdio.h>

#include <sys/types.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef STDC_HEADERS
#include <stdlib.h>
#else
char *malloc ();
char *realloc ();
#endif

#ifdef  SYSVSTR
#include <string.h>
#else
#include <strings.h>
#endif

#include "regex.h"


void *
pvmcompileregex( pattern )
char *pattern;
{
	struct re_pattern_buffer *pattbuff;

	char *patterr;

	pattbuff = (struct re_pattern_buffer *) malloc(
		sizeof( struct re_pattern_buffer ) );

	pattbuff->translate = (char *) NULL;
	pattbuff->fastmap = (char *) NULL;
	pattbuff->buffer = (unsigned char *) NULL;
	pattbuff->allocated = (long) 0;

	patterr = (char *) re_compile_pattern( pattern,
			strlen( pattern ), pattbuff );

	if ( patterr ) {
		printf( "Error Compiling Pattern \"%s\":\n\t%s\n",
				pattern, patterr );

		return( (void *) NULL );
	}

	return( (void *) pattbuff );
}


int
pvmmatchregex( vpattbuff, str )
void *vpattbuff;
char *str;
{
	struct re_pattern_buffer *pattbuff;

	int len;

	pattbuff = (struct re_pattern_buffer *) vpattbuff;

	len = strlen( str );

	return( ( re_match( pattbuff, str, len, 0,
		(struct re_registers *) NULL ) == len ) ? 1 : 0 );
}


void
pvmfreeregex( vpattbuffp )
void **vpattbuffp;
{
	struct re_pattern_buffer *pattbuff;

	if ( vpattbuffp == NULL )
		return;

	pattbuff = (struct re_pattern_buffer *) *vpattbuffp;
	*vpattbuffp = (void *) NULL;

	regfree( (regex_t *) pattbuff );

	free( pattbuff );
}

