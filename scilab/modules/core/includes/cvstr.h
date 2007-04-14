/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Sylvestre LEDRU */
/*-----------------------------------------------------------------------------------*/
#ifndef __CVSTR_H__
#define __CVSTR_H__
/**
 * Convert one ascii char to Scilab internal code 
 *     the big table (pure) ascii -> scilab code 
 * @param c
 * @param c_len
 * @return 
 */

integer C2F(getfastcode)(unsigned char *c, unsigned long c_len) ;

#endif /* __CVSTR_H__ */
