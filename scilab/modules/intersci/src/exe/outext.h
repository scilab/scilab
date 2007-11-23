/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Sylvestre LEDRU */
/*--------------------------------------------------------------------------*/ 
void OutExtCommon  __PARAMS((FILE *f,VARPTR var,int insidelist,int nel));

void OutExtCOLUMN(FILE *f,VARPTR var,int insidelist,int nel);
void OutExtROW(FILE *f,VARPTR var,int insidelist,int nel);
void OutExtVECTOR(FILE *f,VARPTR var,int insidelist,int nel);
void OutExtMATRIX(FILE *f,VARPTR var,int insidelist,int nel);
void OutExtSCALAR(FILE *f,VARPTR var,int insidelist,int nel);
void OutExtSTRING(FILE *f,VARPTR var,int insidelist,int nel);
void OutExtSPARSE(FILE *f,VARPTR var,int insidelist,int nel);
void OutExtIMATRIX(FILE *f,VARPTR var,int insidelist,int nel);
void OutExtBMATRIX(FILE *f,VARPTR var,int insidelist,int nel);
void OutExtBMATRIX1(FILE *f,VARPTR var,int insidelist,int nel);
void OutExtPOLYNOM(FILE *f,VARPTR var,int insidelist,int nel);
void OutExtPOINTER(FILE *f,VARPTR var,int insidelist,int nel);
void OutExtSTRINGMAT(FILE *f,VARPTR var,int insidelist,int nel);
void OutExtANY(FILE *f,VARPTR var,int insidelist,int nel);
