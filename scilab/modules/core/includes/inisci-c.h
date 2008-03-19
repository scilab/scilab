/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __INI_C_H__
#define __INI_C_H__

#include "machine.h"

/** 
 * TODO : comment
 * @param rep
 * @return 
 */
int C2F(withscicos)(int *rep);

/** 
 * TODO : comment
 * @param rep
 * @return 
 */
int C2F(withmodelicac)(int *rep);

/** 
 * TODO : comment
 * @param rep
 * @return 
 */
int C2F(withjavasci)(int *rep);

/** 
 * TODO : comment
 * @param rep
 * @return 
 */
int C2F(withmsdos)(int *rep);

/** 
 * TODO : comment
 * @param buf
 * @param nbuf
 * @param lbuf
 * @return 
 */
int C2F(getcomp)(char *buf,int *nbuf,long int lbuf);


/** 
 * TODO : comment
 * @return 
 */
int ExistJavaSciUnix(void);

/** 
 * TODO : comment
 * @return 
 */
BOOL ExistModelicac(void);

/** 
 * TODO : comment
 * @return 
 */
BOOL ExistScicos(void);

/** 
 * TODO : comment
 * @return 
 */
BOOL ExistJavaSciWin(void);

/** 
 * TODO : comment
 * @return 
 */
int SetSci(void);

/** 
 * TODO : comment 
 * @param buf
 * @param nbuf
 * @param lbuf
 * @return 
 */
int C2F(gettmpdir)(char *buf,int *nbuf,long int lbuf);

/** 
 * TODO : comment
 * @param rep
 * @return 
 */
int C2F(withgui)(int *rep);

/**
 * Get the SCI path and initialize the scilab environment path
 * @param buf
 * @param nbuf
 * @param lbuf
 * @return 
 */
int C2F(getsci)(char *buf,int *nbuf,long int lbuf);

/**
* Get the SCIHOME path and initialize the scilab environment path
 * @param buf
 * @param nbuf
 * @param lbuf
 * @return 
*/
int C2F(getscihome)(char *buf,int *nbuf,long int lbuf);


/**
 * Init the stack and a few global variable
 *
 * @param ini     = -1  for silent initialization  = -3  for special io initialization 
 * @param memory  initial stack size (memory allocated)
 * @param ierr flag
 */
int C2F(inisci)(int *ini,int *memory,int *ierr);

/**
 * Get default value of global scilab stacksize
 *
 * @param[out] default globalstacksize
 */
int C2F(getdefaultgstacksize)(int *defaultsize);

/**
 * Get default value of scilab stacksize
 *
 * @param[out] default stacksize
 */
int C2F(getdefaultstacksize)(int *defaultsize);

#endif /* __INI_C_H__ */
