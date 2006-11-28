/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Ledru Sylvestre */
/*-----------------------------------------------------------------------------------*/ 


int C2F(withpvm)(int *rep);
int C2F(withgtk)(int *rep);
int C2F(withscicos)(int *rep);
int C2F(withmodelicac)(int *rep);
int C2F(withjavasci)(int *rep);
int C2F(withmsdos)(int *rep);
int C2F(getcomp)(char *buf,int *nbuf,long int lbuf);

int ExistJavaSciUnix(void);
BOOL ExistScicos(void);
BOOL ExistJavaSciWin(void);
void SetWITH_GUI(int ON);
BOOL  GetWITH_GUI(void);
BOOL ExistModelicac(void);
int SetSci(void);
int C2F(getsci)(char *buf,int *nbuf,long int lbuf);
int C2F(gettmpdir)(char *buf,int *nbuf,long int lbuf);
int C2F(withgui)(int *rep);
