/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
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
