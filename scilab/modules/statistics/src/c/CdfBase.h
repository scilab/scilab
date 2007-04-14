/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Sylvestre LEDRU */
/*-----------------------------------------------------------------------------------*/

#ifndef __CDFBASE_H__
#define __CDFBASE_H__

/**
 * TODO : needs comments
 * @param fname
 * @param inarg
 * @param oarg
 * @param callpos
 * @param option
 * @param errnames
 * @param which
 * @param (*fonc)()
 * @param (*foncErr)()
 * @return
 */

int  CdfBase(char *fname,int inarg,int oarg,int *callpos,char *option,char *errnames,int which,int (*fonc)(),void (*foncErr)() );

#endif /* __CDFBASE_H__ */
