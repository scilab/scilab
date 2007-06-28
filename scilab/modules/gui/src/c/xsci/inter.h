/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Sylvestre Ledru */
/*-----------------------------------------------------------------------------------*/

#define MAXBUFFSIZE 256
static char buffstring[MAXBUFFSIZE];
static int lenbuffstring;

int IntoEmacs(void );

void sci_tk_activate(void);

void Click_menu(int n);

void str_to_xterm ( register char * string, int nbytes);
