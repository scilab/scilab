/*--------------------------------------------------------------------------*/
/* INRIA 2008 */
/* Sylvestre LEDRU */
/*--------------------------------------------------------------------------*/
#ifndef __GETDEBUGINFO_H__
#define __GETDEBUGINFO_H__

/**
 * The commun stuff for the debug info retrieving
 */

#define NB_DEBUG_ELEMENT 255

typedef struct debugMessage {
	char *description;
	char *value;
} debug_message;

#endif /* __GETDEBUGINFO_H__ */
/*--------------------------------------------------------------------------*/

