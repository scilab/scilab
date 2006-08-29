void CreMATRIX __PARAMS((FILE *f, VARPTR var));
void CreCommon __PARAMS((FILE *f, VARPTR var));
void CreSTRING __PARAMS((FILE *f, VARPTR var));
void CreBMATRIX __PARAMS((FILE *f, VARPTR var));
void CreDIMFOREXT __PARAMS((FILE *f, VARPTR var));
void CreVECTOR __PARAMS((FILE *f, VARPTR var));
void CreCOLUMN __PARAMS((FILE *f, VARPTR var));
void CreSPARSE __PARAMS((FILE *f, VARPTR var));
void CreIMATRIX __PARAMS((FILE *f, VARPTR var));
void CrePOINTER __PARAMS((FILE *f, VARPTR var));
void CreSTRINGMAT __PARAMS((FILE *f, VARPTR var));
void CreSCALAR_old __PARAMS((FILE *f, VARPTR var));
void CreSCALAR __PARAMS((FILE *f, VARPTR var));
void CreANY __PARAMS((FILE *f, VARPTR var));
void CreEMPTY  __PARAMS((FILE *f, VARPTR var));

 
typedef  struct  {
  int type;
  void  (*fonc) __PARAMS((FILE *f, VARPTR var));} CreRhsTab ;

extern CreRhsTab CRERHSTAB[];


