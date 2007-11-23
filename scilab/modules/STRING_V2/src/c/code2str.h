/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/*--------------------------------------------------------------------------*/
#ifndef __CODE2STR__
#define __CODE2STR__

/**
* code2str  returns character string associated with Scilab integer codes
* @param[out] Output_Matrix vector of character integer codes
* @param[in] Number_of_Row
* @param[in] Number_of_Col
* @param[in] a character string 
*/
void code2str(char **Output_Matrix,int Number_of_Row, int Number_of_Col, int *Input_Matrix);

/**
* str2code  return scilab integer codes associated with a character string 
* @param[out] Output_Matrix  vector of character integer codes 
* @param[in] Input_Matrix a character string 
* @return size of Output_Matrix
*/
int str2code(int *Output_Matrix, char **Input_Matrix);

#endif /* __CODE2STR__ */
/*--------------------------------------------------------------------------*/
