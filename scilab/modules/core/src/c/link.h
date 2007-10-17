/* Copyright INRIA */
#ifndef __LINK_H__
#define __LINK_H__

/**
 * <long-description>
 *
 * @param op    
 * @param realop    
 * @return <ReturnValue>
 */
int SearchInDynLinks (char *op, void (**realop)());

/**
 * <long-description>
 *
 * @param void  
 */
void SciLinkInit (void);

/**
 * <long-description>
 *
 * @param iflag 
 * @param rhs   
 * @param ilib  
 * @param char *files[] 
 * @param char *en_names[]  
 * @param strf  
 */
void SciLink (int iflag,int *rhs,int *ilib,char *files[],char *en_names[],char *strf);

/**
 * <long-description>
 *
 * @param buf   
 * @param irep  
 */
void C2F(iislink) (char *buf, integer *irep);

/**
 * <long-description>
 *
 * @param void  
 */
void ulinkall(void);

/**
 * <long-description>
 *
 * @param i 
 */
void C2F(isciulink)(integer *i);

/**
 * <long-description>
 *
 * @param descla    
 * @param ptrdescla 
 * @param nvla  
 * @param desc  
 * @param ptrdesc   
 * @param nv    
 * @param strf  
 * @param ilib  
 * @param iflag 
 * @param rhs   
 */
void C2F(iscilink)(int *descla, int *ptrdescla, int *nvla, int *desc, int *ptrdesc, int *nv, char *strf, int *ilib, int *iflag, int *rhs);

/**
 * <long-description>
 *
 * @param void  
 * @return <ReturnValue>
 */
int C2F(scilinknorhs)(void);

/**
 * <long-description>
 *
 * @param ii    
 * @param realop    
 */
void GetDynFunc(int ii, void (**realop) (/* ??? */));

#endif /*  __LINK_H__ */

