/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2007 - INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef SCI_PVM 
#define SCI_PVM 

#ifdef _MSC_VER
#include "pvm3/include/pvm3.h"
#else
#include "pvm3.h"
#endif
#include "stack-c.h"

#define PVM_CONFIG_FILE "/.pvmd.conf"
#define PVM_MODULE "/modules/pvm/etc"

/**
 * TODO: Add comments
 *
 * @param group 
 * @param l 
 * @param pack  
 * @param n 
 * @param buff  
 * @param msgtag    
 * @param res   
 */
void C2F(scipvmbcast)(char *group, int *l, int *pack, int *n, double *buff, int *msgtag, int *res); 

/**
 * TODO: Add comments
 *
 * @param func  
 * @param l1    
 * @param buff  
 * @param m 
 * @param n 
 * @param msgtag    
 * @param group 
 * @param l2    
 * @param rootginst 
 * @param res   
 */
void C2F(scipvmreduce)(char *func, int *l1,
		       double *buff, int *m, int *n, int *msgtag, 
		       char *group, int *l2, int *rootginst, int *res);


/**
 * get configuration informations 
 *
 * @param nhost 
 * @param narch 
 * @param dtid  
 * @param name  
 * @param arch  
 * @param speed 
 * @param n 
 * @param info  
 */
void C2F(scipvmconfig)(int *nhost, int *narch, int **dtid, 
		       char ***name, char ***arch, int **speed, 
		       int *n, int *info);

/**
 * informations about tasks 
 *
 * @param where 
 * @param ntask 
 * @param tid   
 * @param ptid  
 * @param dtid  
 * @param flag  
 * @param name  
 * @param n 
 * @param info  
 */
void C2F(scipvmtasks)(int *where, int *ntask, 
		      int **tid, int **ptid, int **dtid, int **flag,
		      char ***name, int *n, int *info);


/**
 * TODO: Add comments
 *
 * @param bufid 
 * @param bytes 
 * @param msgtag    
 * @param tid   
 * @param res   
 */
void C2F(scipvmbufinfo)(int *bufid, int *bytes, int *msgtag, int * tid, int *res); 

/**
 * Retrieve the description of the error message from the PVM error code
 *
 * @param err the error code
 * @return The error message
 */
char *scipvm_error_msg(int err); 

/**
 * Start PVM
 *
 * @param res Result of the operation
 * @param hostfile the file containing the list of host file
 * @param hostfile_len
 */
void C2F(scipvmstart)(int *res, char *hostfile, int *hostfile_len);



/**
 * <long-description>
 *
 * @param scipvmsettimer    
 */
void C2F(scipvmsettimer)(int *res);

/**
 * TODO: Add comments
 *
 * @param res
 */
void C2F(scipvmgettimer)(double *res);


/**
 * TODO: Add comments
 *
 * @param task  
 * @param l1    
 * @param win   
 * @param l2    
 * @param where 
 * @param l3    
 * @param ntask 
 * @param tids  
 * @param res   
 */
void C2F(scipvmspawn)(char *task,  int *l1, 
		      char *win,   int *l2,
		      char *where, int *l3, 
		      int *ntask,  int *tids, int *res);


/**
 * TODO: Add comments
 *
 * @param task  
 * @param l1    
 * @param ntask 
 * @param where 
 * @param l3    
 * @param tids  
 * @param res   
 */
void C2F(scipvmspawnindependent)(char *task,  int *l1, 
			    int *ntask,
			    char *where, int *l3, 
			    int *tids, int *res);

/**
 * TODO: Add comments
 *
 * @param beginvar  
 * @param maxsize   
 * @param size  
 * @param tid   
 * @param tag   
 * @param res   
 */
void C2F(scipvmrecv)(double *beginvar, int *maxsize, int *size,
		int *tid,  int *tag, int *res);

/**
 * TODO: Add comments
 *
 * @param tid   
 * @param tag   
 * @param buff  
 * @param res   
 */
void C2F(scipvmrecvvar)(int *tid,  int *tag, char *buff, int *res);


/**
 * TODO: Add comments
 *
 * @param tids  
 * @param p 
 * @param buff  
 * @param msgtag    
 * @param res   
 */
void C2F(scipvmsendvar)(int *tids, int *p, char *buff, 
			 int *msgtag, int *res);

/**
 * TODO: add comments
 *
 * @param taskId The task identification
 * @param p 
 * @param pack  
 * @param n 
 * @param buff  
 * @param msgtag    
 * @param res   
 */
void C2F(scipvmsend)(int *taskId, int *p, 
		     int *pack, int *n, 
		     double *buff,
		     int *msgtag, int *res);

/**
 * TODO: add comments
 *
 * @param k 
 */
void C2F(scipvmscitof77)(int *k);

/**
 * TODO: add comments
 *
 * @param k 
 */
void C2F(scipvmf77tosci)(int *k);

/**
 * Halt PVM
 *
 * @param res   
 */
void C2F(scipvmhalt)(int *res);

/**
 * TODO: add comments
 *
 * @param n 
 * @param ip    
 * @param op    
 */
void ccomplexf(int *n, double **ip, double *op); 

/** 
 *
 * Given a scilab variable, stored in the stack at the position k (in 
 * lstk) this function returns a "packing" vector pack for pvm. 
 * A scilab variable is a consecutive memory region formed by a 
 * succession of int and double precision vectors. 
 * n must be initialized before calling pak_object_info
 * varpak returns a vector which contains the size of this int 
 * and double precision vectors. pack contains a sequence of pairs of 
 * ints:  pack=[i1,r1,i2,r2,...,in,rn] 
 * il is a number of int words 
 * rl is a number of double precision word 
 * @param k    : position in lstk
 * @param pack : contains a sequence of pairs of ints. 
 * @param n    : is the returned length of pack 
 * @param nMax : is the Maximum size allowed for pack  
 * @param ierr : 0 OK 
 * @param ierr : 1 nMax is not enough 
 * @param ierr : 2 unknown data type 
 * @return 0 (no comment)
 */
int C2F(varpak)(int *k, int *pack, int *n, int *nMax, int *ierr); 

/**
 * Return the error message
 *
 * @param n the code of the error
 * @return The error message
 */
char *pvm_geterror(int n);

/**
 * TODO: add comments
 *
 * @param fname 
 * @param err   
 * @param fname_len 
 */
void pvm_error_check(char *fname,int err, unsigned long fname_len);

#endif 
