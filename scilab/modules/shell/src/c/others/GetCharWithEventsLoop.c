/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "GetCharWithEventsLoop.h"
#include "sciprint.h"
#include "MALLOC.h"
#include "core_math.h" /* Max */
/*-----------------------------------------------------------------------------------*/ 
#define LF                    0x000a
/*-----------------------------------------------------------------------------------*/ 
static fd_set Select_mask_ref, select_mask, Write_mask_ref, write_mask;
static int inter_max_plus1 = 0;
static int Xsocket=0, fd_in=0, fd_out=0, fd_err=0 ;
static int isingleton = 0;
/*-----------------------------------------------------------------------------------*/ 
static void initializeScilabMask(void);
static int IntoEmacs(void);
/*-----------------------------------------------------------------------------------*/ 
int GetCharWithEventsLoop(int interrupt)
{
	register int i = 0;
	static struct timeval select_timeout;
	static int state = 0;

	if (!isingleton)
	{
		initializeScilabMask();
		isingleton++;
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
		i = select(inter_max_plus1, &select_mask, &write_mask, (fd_set *)NULL, &select_timeout);
		if (i < 0) 
		{
			if (errno != EINTR) /* EINTR  A signal was caught. */
			{ 
				sciprint(_("Error.\n"));
				exit(0);
				continue;
			}
		} 

		/* if there's something to output */
		if ( FD_ISSET(fd_out,&write_mask)) fflush(stdout); 
		if ( FD_ISSET(fd_err,&write_mask)) fflush(stderr); 

		/* if there's something to read */
		if (FD_ISSET(fd_in,&select_mask)) state=1;
		else  if (!IntoEmacs()) state=0;

		if (state) 
		{
			i = getchar();
			ScilabEventsLoop();
			if (i == LF) state=0;
			return i ;
		}
		else ScilabEventsLoop();
		if (interrupt&&(ismenu()==1)) return(-1);
	}
}
/*-----------------------------------------------------------------------------------*/ 
static void initializeScilabMask( void)
{
	fd_in = fileno(stdin) ;
	fd_out = fileno(stdout);
	fd_err = fileno(stderr);

	FD_ZERO(&Select_mask_ref);
	FD_SET(fd_in , &Select_mask_ref);
	FD_SET(Xsocket, &Select_mask_ref);
	FD_ZERO(&Write_mask_ref);

	inter_max_plus1 = Max(fd_in,Xsocket);      
	inter_max_plus1 = Max(fd_out,inter_max_plus1);
	inter_max_plus1 = Max(fd_err,inter_max_plus1);
	inter_max_plus1++;
}  
/*-----------------------------------------------------------------------------------*/ 
static int IntoEmacs(void )
{
	return(strcmp(getenv("TERM"),"dumb")==0);
}
/*-----------------------------------------------------------------------------------*/ 
