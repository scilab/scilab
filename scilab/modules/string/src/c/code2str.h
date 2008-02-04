/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/*--------------------------------------------------------------------------*/
#ifndef __CODE2STR__
#define __CODE2STR__

/**
* code2str  returns character string associated with Scilab integer codes
* @param[out] Output_Matrix vector of character 
* @param[in] scilab codes of a string
* @param[in] dimension of Input_Matrix
*/
void code2str(char **Output_Matrix, int *Input_Matrix,int Dim_Input_Matrix);


/**
* str2code  return scilab integer codes associated with a character string 
* @param[out] Output_Matrix  vector of character integer codes 
* @param[in] Input_Matrix a character string 
*/
void str2code(int *Output_Matrix, char **Input_Matrix);

#endif /* __CODE2STR__ */
/*--------------------------------------------------------------------------*/
