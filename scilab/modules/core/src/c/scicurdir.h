/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Sylvestre LEDRU */
/*-----------------------------------------------------------------------------------*/

/*******************************
 * Changes scilab current directory 
 * @param path : the path where Scilab should go
 * @param[out] err : the error code
 *******************************/
int C2F(scichdir)(char *path,int *err);

/*******************************
 * Get scilab current directory 
 * @param[out] path : the current path
 * @param[out] lpath : the length of the char * path
 * @param[out] err : the error code
 *******************************/
int C2F(scigetcwd)(char **path,int *lpath,int *err);

