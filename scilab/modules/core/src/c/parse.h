/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Ledru Sylvestre */
/*-----------------------------------------------------------------------------------*/

typedef int logical;
void Msgs(int n,int ierr);
void SciError(int n);
logical Compil(int code,int * val1,int val2,int val3,int val4);
logical Eptover(int n);
logical Ptover(int n);
int C2F(syncexec)(char *str, int *ns, int *ierr, int *seq, long int str_len);
void C2F(parsecomment)(void);
