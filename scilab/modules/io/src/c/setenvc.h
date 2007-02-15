/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Sylvestre LEDRU */
/*-----------------------------------------------------------------------------------*/
/**
 * Set in the system environment a variable to a value
 * @param[in] string the name of the variable
 * @param[in] value the value of the variable "string"
 * @return 0 if there is a problem otherwise 1 
 */
int setenvc(char *string,char *value);
