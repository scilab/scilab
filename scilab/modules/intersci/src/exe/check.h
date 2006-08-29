void CheckMATRIX __PARAMS((FILE *f, VARPTR var, int flag));
void CheckCom __PARAMS((FILE *f, VARPTR var, int flag));
void CheckSTRING __PARAMS((FILE *f, VARPTR var, int flag));
void CheckBMATRIX __PARAMS((FILE *f, VARPTR var, int flag));
void CheckIMATRIX __PARAMS((FILE *f, VARPTR var, int flag));
void CheckSPARSE __PARAMS((FILE *f, VARPTR var, int flag));
void CheckSTRINGMAT __PARAMS((FILE *f, VARPTR var, int flag));
void CheckROW __PARAMS((FILE *f, VARPTR var, int flag));
void CheckCOLUMN __PARAMS((FILE *f, VARPTR var, int flag));
void CheckVECTOR __PARAMS((FILE *f, VARPTR var, int flag));
void CheckPOLYNOM __PARAMS((FILE *f, VARPTR var, int flag));
void CheckSCALAR __PARAMS((FILE *f, VARPTR var, int flag));
void CheckPOINTER __PARAMS((FILE *f, VARPTR var, int flag));
void CheckANY __PARAMS((FILE *f, VARPTR var, int flag));
void CheckLIST __PARAMS((FILE *f, VARPTR var, int flag) );
void CheckTLIST __PARAMS((FILE *f, VARPTR var, int flag));
void CheckSEQUENCE __PARAMS((FILE *f, VARPTR var, int flag));
void CheckEMPTY __PARAMS((FILE *f, VARPTR var, int flag));
void CheckWORK __PARAMS((FILE *f, VARPTR var, int flag));
void CheckDIMFOREXT __PARAMS((FILE *f, VARPTR var, int flag));
void Check __PARAMS((FILE *f, VARPTR var, int nel));
void CheckSquare __PARAMS((FILE *f, VARPTR var, char *str1,char *));
 
typedef  struct  {
  int type;
  void   (*fonc) __PARAMS((FILE *f, VARPTR var, int flag )) ;} CheckRhsTab ;

extern CheckRhsTab CHECKTAB[];

