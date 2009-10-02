/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2009 - DIGITEO - Allan CORNET 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __CALL_SCILAB_H__
#define __CALL_SCILAB_H__


#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------------------------------------------------*/
#include "machine.h"
#include "BOOL.h"
/*--------------------------------------------------------------------------*/
/**
 * Disable TCL/TK and graphic interfaces 
 * Scilab no GUI no TCL/TK "kernel mode" 
*/
void DisableInteractiveMode(void);

/**
 * Initialize Scilab
 * @param SCIpath define SCI environment variable : Default --> NULL
 * @param ScilabStartup path & filename of scilab.start : Default --> NULL 
 * @param Stacksize : Default --> NULL 
 * @return TRUE if it is OK else FALSE
*/
BOOL StartScilab(char *SCIpath,char *ScilabStartup,int *Stacksize);

/**
 * Terminate Scilab
 * @param ScilabQuit path & filename of scilab.quit : Default --> NULL 
 * @return TRUE if it is OK else FALSE 
*/
BOOL TerminateScilab(char *ScilabQuit);

/**
 * Send a job to scilab 
 * @param job the Scilab Job
 * @return error code operation 0 : OK
*/
int SendScilabJob(char *job); 

/**
 * Send jobs to scilab 
 * @code 
 * 	jobs[0] : a = 1;
 * 	jobs[1] : b = 3;
 *	jobs[2] : c = a + b;
 *	SendScilabJobs(jobs,3);
 * @endcode
 * @return last error code operation 0 : OK
**/
int SendScilabJobs(char **jobs,int numberjobs);

/**
 * Returns last job send to scilab by SendScilabJobs or SendScilabJob
 * @param[out] JOB the last job
 * @param[out] nbcharsJOB the size of JOB
 * @code
 * Example :
 * jobs[0] : a = 1;
 * jobs[1] : b = V_NOT_EXIST;
 * jobs[2] : c = a + b;
 * if (SendScilabJobs(jobs,3))
 * {
 * 	char lastjob[4096]; // bsiz in scilab 
 *	if (GetLastJob(lastjob,4096))
 *	{
 *		printf("%s\n",lastjob);
 *	}
 * }
 * @endcode
 * @return 
**/
BOOL GetLastJob(char *JOB,int nbcharsJOB);

/**
* This procedure is the entry point to Scilab's event loop 
*/
void ScilabDoOneEvent(void);

/**
 * Get the information is a graphic windows is opened or not
 * @Return Returns TRUE if a graphic windows is opened 
*/
int ScilabHaveAGraph(void);

/********************* DATATYPES MANAGEMENT FUNCTIONS ************/

/****** READ FUNCTIONS ******/

/** 
 * This function is now obsolete
 * please us creadmat 
 */
int C2F(readmat) (char *name__, int *m, int *n, double *scimat, unsigned long name_len);


/**
 * Read a matrix in scilab's internal memory 
 * calling sequence 
 *     logic=creadmat('matrixname',m,n,scimat)
 * @example
 *    Amat is a real 2 x 3 scilab matrix 
 *    your subroutine should be as follows: 
 *    subroutine mysubr(...) 
 *    ... 
 * @code
 *    call creadmat('Amat',m,n,scimat) 
 *    => m=3 , n=2, and scimat(1)=Amat(1,1) 
 *                      scimat(2)=Amat(2,1) 
 *                      scimat(3)=Amat(3,1) 
 *                      scimat(4)=Amat(1,2) ... 
 *                      scimat(5)=Amat(3,2) 
 *                      scimat(6)=Amat(3,2) 
 * @endcode
 * @param name__ character string; name of the scilab variable. 
 * @param m number of rows
 * @param n number of columns
 * @param scimat  matrix entries stored columnwise
 * @param name_len
 * @return if the operation successed (true) or not (false)
 */
int C2F(creadmat) (char *name__, int *m, int *n, double *scimat, unsigned long name_len);

/**
 * Read a boolean matrix in scilab's memory
 * calling sequence 
 *     logic=creadbmat('matrixname',m,n,scimat)
 * @example
 *    Amat is a real 2 x 3 scilab matrix 
 *    your subroutine should be as follows: 
 *    subroutine mysubr(...) 
 *    ... 
 * @code
 *    call creadbmat('Amat',m,n,scimat) 
 *    => m=3 , n=2, and scimat(1)=Amat(1,1) 
 *                      scimat(2)=Amat(2,1) 
 *                      scimat(3)=Amat(3,1) 
 *                      scimat(4)=Amat(1,2) ... 
 *                      scimat(5)=Amat(3,2) 
 *                      scimat(6)=Amat(3,2) 
 * @endcode
 * @param name__ character string; name of the scilab variable. 
 * @param m number of rows
 * @param n number of columns
 * @param scimat boolean matrix entries stored columnwise
 * @param name_len
 * @return if the operation successed (true) or not (false)
 */
int C2F(creadbmat)(char *namex, int *m, int *n, int *scimat, unsigned long name_len);

/**
 * Read a matrix of complex from Scilab memory 
 */
int C2F(creadcmat) (char *name__, int *m, int *n, double *scimat, unsigned long name_len);

/** 
 * This function is now obsolete
 * please us creadchain
 */
int C2F(readchain) (char *name__, int *itslen, char *characterString, unsigned long name_len, unsigned long characterString_len);

/**
 * Read a string in scilab's memory
 * calling sequence 
 *     logic=creadchain('matrixname',size,string)
 * @param name__ character string; name of the scilab variable. 
 * @param itslen lenght of the string
 * @param chai the future string
 * @param name_len strlen of name__ (fortran needs it)
 * @param chai_len strlen of chai (fortran needs it)
 * @return if the operation successed (true) or not (false)
 */
int C2F(creadchain)  (char *name__, int *itslen, char *characterString, unsigned long name_len, unsigned long characterString_len);

/**
 * Read a string from a matrix of string in scilab's internal memory 
 * calling sequence 
 *     logic=creadchains('matrixname',size,string)
 * @param name__ character string; name of the scilab variable. 
 * @param indiceRow The row position
 * @param indiceCol The column position
 * @param itslen The length of the retrieved string
 * @param chai the string retrieved
 * @param name_len strlen of name (fortran needs it)
 * @param chai_len strlen of chai (fortran needs it)
 * @return if the operation successed (true) or not (false)
 */
int C2F(creadchains)  (char *name__, int *indiceRow, int *indiceCol, int *itslen, char *chai, unsigned long name_len, unsigned long chai_len);


/****** QUERY FUNCTIONS ******/
/**
* Get length of strings by name
* @param[in] name_ name of the scilab variable
* @param[out] m number of rows of the matrix 
* @param[out] n number of columns of the matrix 
* @return array of length or NULL
*/
int *GetLengthStringMatrixByName(char *name_, int *m, int *n);

/**
 * Get pointer on a named matrix 
 * @param namex the name of the Scilab variable
 * @param m number of rows of the matrix 
 * @param n number of columns of the matrix 
 * @param lp
 * @param name_len strlen of name__ (Fortran needs it)
 * @return if the operation successed (true) or not (false)
*/
int C2F(matptr)  (char *name__, int *m, int *n, int *lp, unsigned long name_len);

/**
 * Get pointer on a named matrix 
 * @param namex the name of the Scilab variable
 * @param m number of rows of the matrix 
 * @param n number of columns of the matrix 
 * @param lp
 * @param name_len strlen of name__ (Fortran needs it)
 * @return if the operation successed (true) or not (false)
*/
int C2F(cmatptr)  (char *name__, int *m, int *n, int *lp, unsigned long name_len);

/**
 * Get pointer on a named complex matrix 
 * @param namex the name of the Scilab variable
 * @param m number of rows of the matrix 
 * @param n number of columns of the matrix 
 * @param lp
 * @param name_len strlen of name__ (Fortran needs it)
 * @return if the operation successed (true) or not (false)
*/
int C2F(cmatcptr)  (char *name__, int *m, int *n, int *lp, unsigned long name_len);

/**
 * Get pointer on a named string matrix 
 * @param namex the name of the Scilab variable
 * @param m number of rows of the matrix 
 * @param n number of columns of the matrix 
 * @param ix
 * @param j
 * @param lp
 * @param nlr
 * @param name_len strlen of name__ (Fortran needs it)
 * @return if the operation successed (true) or not (false)
*/
int C2F(cmatsptr)  (char *name__, int *m, int *n, int *ix, int *j, int *lp, int *nlr, unsigned long name_len);


/**
 * Get pointer on a named boolean matrix 
 * @param namex the name of the Scilab variable
 * @param m number of rows of the matrix 
 * @param n number of columns of the matrix 
 * @param lp
 * @param name_len strlen of name__ (Fortran needs it)
 * @return if the operation successed (true) or not (false)
*/
int C2F(cmatbptr)(char *namex, int *m,int *n,int *lp, unsigned long name_len);


/**
 * Returns length of a "chain variable" in scilab
 * example :
 * in scilab --> str = "abcdefghijklmnopqrstuvwxyz";
 * in C getlengthchain("str") returns 26
 * @param namex
 * @return  -1 if error
*/ 
int getlengthchain(char *namex);


/****** WRITE FUNCTIONS ******/
/**
 * cwritemat writes vector/matrix in scilab's memory
 * logic=cwritemat('matrixname'//char(0),m,n,mat)
 * @param name__ character string; name of the scilab variable ( null terMinated) 
 * @param m number of rows 
 * @param n number of columns 
 * @param mat matrix entries stored columnwise in Scilab object
 * @param name_len strlen of name__ (Fortran needs it)
 * @return if the operation successed (true) or not (false)
 */
int C2F(cwritemat)  (char *name__, int *m, int *n, double *mat, unsigned long name_len);


/**
 * cwritecmat writes vector/matrix of complex in scilab's memory
 * logic=cwritecmat('matrixname'//char(0),m,n,mat)
 * @param name__ character string; name of the scilab variable ( null terMinated) 
 * @param m number of rows 
 * @param n number of columns 
 * @param mat complex matrix entries stored columnwise in Scilab object
 * @param name_len strlen of name__ (Fortran needs it)
 * @return if the operation successed (true) or not (false)
 */
int C2F(cwritecmat)  (char *name__, int *m, int *n, double *mat, unsigned long name_len);


/**
 * Write a string into the Scilab memory
 *
 * @param name__ name of the scilab variable
 * @param m length of the string itself
 * @param chai the string itself
 * @param name_len strlen of name__ (fortran needs it)
 * @param chai_len strlen of chai (fortran needs it)
 * @return if the operation successed (true) or not (false)
 */
int C2F(cwritechain)  (char *name__, int *m, char *chai, unsigned long name_len, unsigned long chai_len);

/**
 * cwritemat writes vector/matrix of boolean in scilab's memory
 * logic=cwritebmat('matrixname'//char(0),m,n,mat)
 * @code
 *	int A[]={1,0,0,1};   // Declare the matrix 
 * // NOTE that it is an array of int and not an array of double 
 *		int rowA=1, colA=4; // Size of the matrix 
 *	char variableName[]="A";
 *
 *	C2F(cwritebmat)(variableName, &rowA, &colA, A,strlen(variableName)); // Write it into Scilab's memory 
 * @endcode
 * @param name__ character string; name of the scilab variable ( null terMinated) 
 * @param m number of rows 
 * @param n number of columns 
 * @param mat boolean matrix entries stored columnwise in Scilab object
 * @param name_len strlen of name__ (Fortran needs it)
 * @return if the operation successed (true) or not (false)
 */
int C2F(cwritebmat)(char *namex, int *m, int *n, int *mat, unsigned long name_len);

int C2F(str2name)  (char *name__, int *id, unsigned long name_len);
int C2F(objptr)  (char *name__, int *lp, int *fin, unsigned long name_len);

#ifdef __cplusplus
}
#endif
/*--------------------------------------------------------------------------*/
#endif /* __CALL_SCILAB_H__ */
/*--------------------------------------------------------------------------*/
