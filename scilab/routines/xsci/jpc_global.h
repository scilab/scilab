/*****************************************************************************
 *
 *  xdbx - X Window System interface to the dbx debugger
 *
 *  Copyright 1989 The University of Texas at Austin
 *  Copyright 1990 Microelectronics and Computer Technology Corporation
 *
 *  Permission to use, copy, modify, and distribute this software and its
 *  documentation for any purpose and without fee is hereby granted,
 *  provided that the above copyright notice appear in all copies and that
 *  both that copyright notice and this permission notice appear in
 *  supporting documentation, and that the name of The University of Texas
 *  and Microelectronics and Computer Technology Corporation (MCC) not be 
 *  used in advertising or publicity pertaining to distribution of
 *  the software without specific, written prior permission.  The
 *  University of Texas and MCC makes no representations about the 
 *  suitability of this software for any purpose.  It is provided "as is" 
 *  without express or implied warranty.
 *
 *  THE UNIVERSITY OF TEXAS AND MCC DISCLAIMS ALL WARRANTIES WITH REGARD TO
 *  THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 *  FITNESS, IN NO EVENT SHALL THE UNIVERSITY OF TEXAS OR MCC BE LIABLE FOR
 *  ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
 *  RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
 *  CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 *  CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 *  Author:  	Po Cheung
 *  Created:   	March 10, 1989
 *
 *****************************************************************************
 * 
 *  xxgdb - X Window System interface to the gdb debugger
 *  
 * 	Copyright 1990 Thomson Consumer Electronics, Inc.
 *  
 *  Permission to use, copy, modify, and distribute this software and its
 *  documentation for any purpose and without fee is hereby granted,
 *  provided that the above copyright notice appear in all copies and that
 *  both that copyright notice and this permission notice appear in
 *  supporting documentation, and that the name of Thomson Consumer
 *  Electronics (TCE) not be used in advertising or publicity pertaining
 *  to distribution of the software without specific, written prior
 *  permission.  TCE makes no representations about the suitability of
 *  this software for any purpose.  It is provided "as is" without express
 *  or implied warranty.
 *
 *  TCE DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 *  ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT
 *  SHALL TCE BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES
 *  OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 *  WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
 *  ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 *  SOFTWARE.
 *
 *  Adaptation to GDB:  Pierre Willard
 *  XXGDB Created:   	December, 1990
 *
 *****************************************************************************/

/*  global.h:
 *
 *    Contain extern declarations of variables and functions.
 */


#ifdef __STDC__
#ifndef  __PARAMS
#define  __PARAMS(paramlist)		paramlist
#endif
#else	
#ifndef  __PARAMS
#define  __PARAMS(paramlist)		()
#endif
#endif

#include "../machine.h"
#include "jpc_defs.h"

#include "All-extern.h"

/* calldbx.c */

extern void		calldbx();		/* fork child, exec dbx */

/* command.c */

extern void		CreateCommandPanel();
extern void		send_command();
extern char		*get_command();
extern void		insert_command();
extern void		delete_command();

/* datadpy.c */

extern void		print_handler();	/* printing variables */

/* dbx.c */

extern void		debug_init();		/* initial source display */
extern void 		read_dbx();		/* get data from dbx */
extern void 		write_scilab();		/* send data to dbx */
extern void		query_dbx();		/* ask dbx for info */


/* filemenu.c */

extern void		File(),FileE(),FileG();			/* file button callback */

/* handler.c */

extern void		TextSetTopPosition();	/* sets topDisplayPosition */
extern void 		AdjustText();		/* adjust display position */
extern void 		exec_handler();		/* run, cont, next, step */
extern void 		done_handler();		/* run, cont, next, step */
extern void 		stop_at_handler();	/* stop at line */
extern void 		stop_in_handler();	/* stop in function */
extern void 		updown_handler();	/* move up/down the stack */
extern void 		delete_handler();	/* delete stop signs */
extern void 		func_handler();		/* display function */
extern void 		file_handler();		/* display file */
extern void 		debug_handler();	/* debug program */
extern void 		cd_handler();		/* change directory */
extern void 		pwd_handler();		/* print working directory */
extern void 		use_handler();		/* update use directory list */
extern void 		search_handler();	/* display searched string */
extern void 		list_handler();		/* list source code */
extern void 		display_handler();	/* show display variables */

/* parser.c */

extern void		parser_init();		/* initialization */
extern void		parse();		/* parse dbx output */
extern void		filter();		/* modify dbx output */

/* signals.c */
extern void		trap_signals();		/* signal handling for xdbx */

/* signs.c */

extern void		signs_init();		/* initilalize routine */
extern void 		DisplayStop();		/* show stop sign */
extern void 		UpdateStops();		/* update position of stops */
extern void 		ClearStops();		/* mark all stops invalid */
extern void 		RemoveStop();		/* undisplay stop sign */
extern void 		UpdateArrow();		/* update position of arrow */
extern void 		UpdateUpdown();		/* update position of updown */
extern void 		UpdateBomb();		/* update position of bomb */

/* source.c */

extern void		SelectStart();		/* modified select-start */
extern void		SelectEnd();		/* modified select-end */
extern void		SelectWord();		/* my select word */
extern void		PrintSelection();	/* select variable and print */
extern void		Update();		/* update line label */
extern void		source_init();		/* init routine */
extern void 		CreateSourceWindow();
extern int 		LoadFile();		/* display source file */
extern int 		LoadCurrentFile();	/* display source file */
extern char		*GetPathname();		/* get full path name of file */
extern void		MakeDirList();		/* maintain list of dirs */

/* utils.c */

extern int 		TextPositionToLine();	/* convert line # to text pos */
extern int		LineToStop_no();	/* convert line # to stop # */
extern void 		DisableWindowResize();  /* do not allow window resize */
extern void 		bell();			/* sound bell */
extern char 		*concat();		/* substring search */

/* windows.c */


extern void 		UpdateFileLabel();	/* update current file name */
extern void 		UpdateLineLabel();	/* update current line num */
extern void 		UpdateMessageWindow();	/* update xdbx message */

/* extern variables */

extern Display	*display;
extern XtAppContext 	app_con;
extern Widget	toplevel, fileWindow, 
        	sourceForm, sourceWindow, messageWindow, 
		dialogWindow, searchWindow, separator;

extern Cursor	watch;			/* XC_watch cursor */
extern XdbxResources application_resources;	/* application resources */
extern char	xdbxinit[];		/* initialization filename */
extern char	*dbxprompt;		/* dbx prompt string */
extern char	*xdbxprompt;		/* xdbx prompt string */
extern Boolean	Tstartup;
extern Boolean	debug_jpc;

extern FileRec   	*displayedFile;	/* pointer to current file info */
extern Tokens		Token;		/* token structure */
extern PatternRec       dataPattern[]; 	/* data display pattern */

extern Boolean 	Echo;			/* echo dbx output onto window ? */
extern Boolean	Prompt;			/* command or program input ? */
extern Boolean	Parse;			/* flag for parser */
extern Boolean	FalseSignal;		/* real/false signal ? */
extern Boolean	PopupMode;		/* True if graphical display */
extern int	dbxpid;			/* dbx process id */
extern FILE	*dbxfp;			/* file pointer to dbx process */

extern char 	CurrentFile[];		/* string for dbx output */
extern char 	cwd[];			/* current working directory of dbx */

extern Arrow    arrow;			/* arrow widget and mapped info */
extern Updown   updown;			/* updown widget and mapped info */
extern Stops    stops[];		/* stop widget and mapped info */
extern Bomb	bomb;			/* bomb widget and mapped info */
extern Cardinal nstops;			/* number of stops */

