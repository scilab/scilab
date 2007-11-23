/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Sylvestre LEDRU */
/*--------------------------------------------------------------------------*/
#ifndef __SETENVC_H__
#define __SETENVC_H__

/**
 * Set in the system environment a variable to a value
 * @param[in] string the name of the variable
 * @param[in] value the value of the variable "string"
 * @return 0 if there is a problem otherwise 1 
 */
int setenvc(char *string,char *value);

/**
 * TODO : comment 
 * @return 
 */
int getUpdateEnvVar(void);

/**
 * TODO : comment 
 * @param val
 */
void setUpdateEnvVar(int val);

#endif /* __SETENVC_H__ */
/*--------------------------------------------------------------------------*/
