/*  global.h:
 *
 *    Contain extern declarations of variables and functions.
 */

#include "../machine.h"
#include "jpc_defs.h" 
#include "All-extern.h"

/* command.c */

extern void		CreateCommandPanel();
extern void		send_command();
extern char		*get_command();
extern void		insert_command();
extern void		delete_command();

/* datadpy.c */

extern void		print_handler();	/* printing variables */

/* dbx.c */

extern void 		write_scilab();		/* send data to dbx */

/* parser.c */

extern void		parser_init();		/* initialization */
extern void		parse();		/* parse dbx output */
extern void		filter();		/* modify dbx output */

/* signals.c */
extern void		trap_signals();		/* signal handling for xdbx */


