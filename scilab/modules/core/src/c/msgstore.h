/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Ledru Sylvestre */
/*-----------------------------------------------------------------------------------*/ 


/**
 * TODO : comment
 * @param n
 */
int C2F(errstore)(int *n);


/**
 * TODO : comment
 * @param n
 */
int C2F(linestore)(int *n);

/**
 * TODO : comment
 * @param str
 * @param n
 */
int C2F(funnamestore)(char *str,int *n);

/**
 * TODO : comment
 * @param str
 * @param n
 */
int C2F(msgstore)(char *str,int *n);

/**
 * TODO : comment
 */
void C2F(freemsgtable)(void);


/**
 * TODO : comment
 * @param fname
 * @param fname_len
 */
int C2F(lasterror)(char *fname, unsigned long fname_len);


/**
 * TODO : comment
 */
int GetLastErrorCode(void);

