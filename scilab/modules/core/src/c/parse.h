/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Ledru Sylvestre */
/*-----------------------------------------------------------------------------------*/
#ifndef __PARSE_H__
#define __PARSE_H__
/*-----------------------------------------------------------------------------------*/
#include "machine.h"
/*-----------------------------------------------------------------------------------*/

/**
 * Parse a the global variable C2F(cha1).buf (ugly ...)
 *
 * @return <ReturnValue>
 */
int C2F(parse)(void);

/**
 * @TODO : add comment
 *
 * @param n 
 * @param ierr  
 */
void Msgs(int n,int ierr);

/**
 * @TODO : add comment
 *
 * @param n 
 */
void SciError(int n);

/**
 * @TODO : add comment
 *
 * @param n 
 * @return <ReturnValue>
 */
int Eptover(int n);

/**
 * @TODO : add comment
 *
 * @param n 
 * @return <ReturnValue>
 */
int Ptover(int n);

/**
 * @TODO : add comment
 *
 * @param void  
 */
void C2F(parsecomment)(void);
/*-----------------------------------------------------------------------------------*/
#endif /* __PARSE_H__ */
/*-----------------------------------------------------------------------------------*/
