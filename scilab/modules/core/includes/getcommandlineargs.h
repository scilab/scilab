/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GETCOMMANDLINEARGS_H__
#define __GETCOMMANDLINEARGS_H__

/* max of command token */

#define MAXCMDTOKENS 256

/**
* get command line arguments
* @param returns nbargs number of arguments
* @return arguments (char **) 
*/
char ** getCommandLineArgs(int *nbargs);

/**
* save command line arguments
* param argv
* param nbargs
*/
int setCommandLineArgs(char **argv, int nbargs);

#endif /* __GETCOMMANDLINEARGS_H__ */
/*-----------------------------------------------------------------------------------*/ 
