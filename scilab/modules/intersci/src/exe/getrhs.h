void GetMATRIX __PARAMS((FILE *f, VARPTR var, int flag));
void GetCom __PARAMS((FILE *f, VARPTR var, int flag));
void GetSTRING __PARAMS((FILE *f, VARPTR var, int flag));
void GetBMATRIX __PARAMS((FILE *f, VARPTR var, int flag));
void GetIMATRIX __PARAMS((FILE *f, VARPTR var, int flag));
void GetSPARSE __PARAMS((FILE *f, VARPTR var, int flag));
void GetSTRINGMAT __PARAMS((FILE *f, VARPTR var, int flag));
void GetROW __PARAMS((FILE *f, VARPTR var, int flag));
void GetCOLUMN __PARAMS((FILE *f, VARPTR var, int flag));
void GetVECTOR __PARAMS((FILE *f, VARPTR var, int flag));
void GetPOLYNOM __PARAMS((FILE *f, VARPTR var, int flag));
void GetSCALAR __PARAMS((FILE *f, VARPTR var, int flag));
void GetPOINTER __PARAMS((FILE *f, VARPTR var, int flag));
void GetANY __PARAMS((FILE *f, VARPTR var, int flag));
void GetLIST __PARAMS((FILE *f, VARPTR var, int flag) );
void GetTLIST __PARAMS((FILE *f, VARPTR var, int flag));
void GetSEQUENCE __PARAMS((FILE *f, VARPTR var, int flag));
void GetEMPTY __PARAMS((FILE *f, VARPTR var, int flag));
void GetWORK __PARAMS((FILE *f, VARPTR var, int flag));
void GetDIMFOREXT __PARAMS((FILE *f, VARPTR var, int flag));
void Check __PARAMS((FILE *f, VARPTR var, int nel));
void CheckSquare __PARAMS((FILE *f, VARPTR var, char *str1,char *));
 
typedef  struct  {
  int type;
  void   (*fonc) __PARAMS((FILE *f, VARPTR var, int flag )) ;} GetRhsTab ;

extern GetRhsTab RHSTAB[];

