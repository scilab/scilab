/*--------------------------------------------------------------------------*/
/* Allan CORNET */
/* INRIA 2007 */
/*--------------------------------------------------------------------------*/
#ifndef __STRSUBST_H__
#define __STRSUBST_H__

/**
* substitute a character string by another in a character string in string matrix
* @param[in] strings_input
* @param[in] dimension of strings_input
* @param[in] string_to_search
* @param[in] replacement_string
* @return substituted string matrix
*/
char **strsubst(char **strings_input,int strings_dim,char *string_to_search,char *replacement_string);

/**
* substitute a character string by another in a character string
* @param[in] string 
* @param[in] string to search in input_string
* @param[in] replacement_string
* @return substituted string
*/
char *strsub(char* input_string, const char* string_to_search, const char* replacement_string);

#endif /* __STRSUBST_H__ */
