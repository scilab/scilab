/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Ledru Sylvestre */
/*-----------------------------------------------------------------------------------*/ 



int C2F(errstore)(int *n);
int C2F(linestore)(int *n);

int C2F(funnamestore)(char *str,int *n);
int C2F(msgstore)(char *str,int *n);

void C2F(freemsgtable)(void);

int C2F(lasterror)(char *fname, unsigned long fname_len);

int GetLastErrorCode(void);

