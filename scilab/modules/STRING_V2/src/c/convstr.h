/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/*--------------------------------------------------------------------------*/
#ifndef __CONVSTR_H__
#define __CONVSTR_H__

#define LOW 'l'
#define LOW_B 'L'
#define UPPER 'u'
#define UPPER_B 'U'

/**
* convstr case conversion 
* param[in] Input_Matrix Input string matrix
* param[out] Output_Matrix 
* param[in] typ  'l' or 'u'  means low or upper
* param[in] Number_Input   the number of input string matrix
*/
void convstr(char **Input_Matrix, char **Output_Matrix, char typ, int Number_Input);

#endif /* __CONVSTR_H__ */
/*--------------------------------------------------------------------------*/

