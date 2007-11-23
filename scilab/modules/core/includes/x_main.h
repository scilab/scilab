/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Ledru Sylvestre */
/*--------------------------------------------------------------------------*/
#ifndef __X_MAIN_H__
#define __X_MAIN_H__


#define	DEFFONT			"fixed"
#define	DEFBOLDFONT		NULL 	/* no bold font uses overstriking */
#define	DEFBORDER		2
#define	DEFBORDERWIDTH		2

/**
 * TODO: comment
 *
 * @param startup Which script should be started
 * @param lstartup Length the char * startup
 * @param memory Memory we want to allocate
 */
void main_sci (char *startup, int lstartup, int memory);

/**
 * TODO: comment
 *
 * @param argc  
 * @param argv  
 */
void mainscic(int argc, char **argv);



/**
 * TODO: comment
 *
 * @param n 
 */
void sci_sig_tstp(int n);



/**
 * TODO: comment
 *
 * @param void  
 */
void InitXsession(void);

/*----------------------------------------------------------------------------------*/ 

#endif /* #define __X_MAIN_H__ */
/*--------------------------------------------------------------------------*/
