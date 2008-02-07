/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Ledru Sylvestre */
/*--------------------------------------------------------------------------*/ 
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


#endif /* __INI_C_H__ */
