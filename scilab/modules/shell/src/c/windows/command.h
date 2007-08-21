/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifndef __COMMAND_H__
#define __COMMAND_H__

/* maximum number of chars allowed on line */
#define MAX_LINE_LEN 512	

/**
* reads a scilab line 
* @param[in] current prompt
* @param[in] interrupt mode
*/
int read_line (char *prompt, int interrupt);

#endif /* __COMMAND_H__ */
/*-----------------------------------------------------------------------------------*/
