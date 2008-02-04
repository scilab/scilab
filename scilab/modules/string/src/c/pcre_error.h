/*--------------------------------------------------------------------------*/
/* Sylvestre LEDRU */
/* INRIA 2008 */
/*--------------------------------------------------------------------------*/
#ifndef __PCRE_ERROR_H__
#define __PCRE_ERROR_H__


/**
 * 
 * Display a human readable error message (PCRE error).
 * @param fname the name of the Scilab function
 * @param errorCode the error code returned by the pcre function
 */
void pcre_error(char *fname, int errorCode);

#endif

