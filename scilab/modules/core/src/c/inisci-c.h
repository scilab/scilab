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
void SetWITH_GUI(int ON);
BOOL GetWITH_GUI(void);
BOOL ExistModelicac(void);
BOOL ExistScicos(void);
BOOL ExistJavaSciWin(void);
int SetSci(void);

int C2F(gettmpdir)(char *buf,int *nbuf,long int lbuf);
int C2F(withgui)(int *rep);
/**
 * Get the SCI path and initialize the scilab environment path
 *
 */
int C2F(getsci)(char *buf,int *nbuf,long int lbuf);
/**
* Get the SCIHOME path and initialize the scilab environment path
*
*/
int C2F(getscihome)(char *buf,int *nbuf,long int lbuf);
