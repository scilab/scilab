#ifndef __PLOT_H__
#define __PLOT_H__

#define PROMPT "Scilab> "

#define TRUE 1
#define FALSE 0

#define MAX_LINE_LEN 512	/* maximum number of chars allowed on line */
#define MAX_ID_LEN 50		/* max length of an identifier */
#define NO_CARET (-1)

#define OS "MS-Windows "

/* Windows needs to redefine stdin/stdout functions */
#include "wtext.h"

#endif /*  __PLOT_H__ */

