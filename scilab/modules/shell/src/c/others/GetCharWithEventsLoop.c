/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Allan CORNET 
 * Copyright (C) 2007-2008 - INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/select.h>
#include "GetCharWithEventsLoop.h"
#include "localization.h"
#include "sciprint.h"
#include "MALLOC.h"
#include "core_math.h" /* Max */
#include "dynamic_menus.h"
/*--------------------------------------------------------------------------*/
#define LF                    0x000a
/*--------------------------------------------------------------------------*/
static fd_set Select_mask_ref, select_mask, Write_mask_ref, write_mask;
static int inter_max_plus1 = 0;
static int Xsocket=0, fd_in=0, fd_out=0, fd_err=0 ;
static int isingleton = 0;
/*--------------------------------------------------------------------------*/
static void initializeScilabMask(void);
static int IntoEmacs(void);
/*--------------------------------------------------------------------------*/
int GetCharWithEventsLoop(int interrupt)
{
	static struct timeval select_timeout;
	static int state = 0;

	if (!isingleton)
	{
		initializeScilabMask();
		isingleton=1;
	}

	for( ; ; )
	{
		fflush(stdout);
		fflush(stderr);

		/* Initialize masks  */
		select_mask = Select_mask_ref;
		write_mask  = Write_mask_ref;

		select_timeout.tv_sec = 0;
		select_timeout.tv_usec = 10;

		if (select(inter_max_plus1, &select_mask, &write_mask, (fd_set *)NULL, &select_timeout)==-1)
		{
			int errnum = errno;
			if (errnum == EINTR) /* EINTR  A signal was caught. */
			{
			  exit(errnum);
			}
			sciprint(_("%s: An error occurred: %s\n"),"GetCharWithEventsLoop",strerror(errnum));
		}

		/* if there's something to output */
		if ( FD_ISSET(fd_out,&write_mask)) {
			fflush(stdout);
		}
		if ( FD_ISSET(fd_err,&write_mask)) {
			fflush(stderr);
		}

		/* if there's something to read */
		if (FD_ISSET(fd_in,&select_mask)) {
			state=1;
		} else {
			if (!IntoEmacs()) {
				state=0;
			}
		}

		if (state)
		{
			int i = getchar();
			if (i == LF) {
				state=0;
			}
			return i ;
		}
		if (interrupt && (ismenu()==1)) {
			return -1;
		}
	}
}
/*--------------------------------------------------------------------------*/
static void initializeScilabMask(void)
{
	/*
	 * Examines the  argument  stream  and  returns  its integer descriptor.
	 */
	fd_in = fileno(stdin) ;
	fd_out = fileno(stdout);
	fd_err = fileno(stderr);

	FD_ZERO(&Select_mask_ref); /* clears the set. */
	FD_SET(fd_in , &Select_mask_ref); /* Add the file descriptor to the set */
	FD_SET(Xsocket, &Select_mask_ref);
	FD_ZERO(&Write_mask_ref); /* clears the set. */


	/* This is an integer  one  more  than  the  maximum  of  any  file
	 * descriptor  in  any  of  the sets.
	 * See man select for more information
	 */
	inter_max_plus1 = Max(fd_in,Xsocket);
	inter_max_plus1 = Max(fd_out,inter_max_plus1);
	inter_max_plus1 = Max(fd_err,inter_max_plus1);
	inter_max_plus1++;
}
/*--------------------------------------------------------------------------*/
static int IntoEmacs(void)
{
	return(strcmp(getenv("TERM"),"dumb")==0);
}
/*--------------------------------------------------------------------------*/
