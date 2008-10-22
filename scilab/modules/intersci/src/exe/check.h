/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2000-2008 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

void CheckMATRIX(FILE *f, VARPTR var, int flag);
void CheckCom (FILE *f, VARPTR var, int flag);
void CheckSTRING(FILE *f, VARPTR var, int flag);
void CheckBMATRIX(FILE *f, VARPTR var, int flag);
void CheckIMATRIX(FILE *f, VARPTR var, int flag);
void CheckSPARSE (FILE *f, VARPTR var, int flag);
void CheckSTRINGMAT(FILE *f, VARPTR var, int flag);
void CheckROW(FILE *f, VARPTR var, int flag);
void CheckCOLUMN(FILE *f, VARPTR var, int flag);
void CheckVECTOR(FILE *f, VARPTR var, int flag);
void CheckPOLYNOM(FILE *f, VARPTR var, int flag);
void CheckSCALAR(FILE *f, VARPTR var, int flag);
void CheckPOINTER(FILE *f, VARPTR var, int flag);
void CheckANY(FILE *f, VARPTR var, int flag);
void CheckLIST(FILE *f, VARPTR var, int flag) ;
void CheckTLIST(FILE *f, VARPTR var, int flag);
void CheckSEQUENCE(FILE *f, VARPTR var, int flag);
void CheckEMPTY(FILE *f, VARPTR var, int flag);
void CheckWORK(FILE *f, VARPTR var, int flag);
void CheckDIMFOREXT(FILE *f, VARPTR var, int flag);
void Check(FILE *f, VARPTR var, int nel);
void CheckSquare(FILE *f, VARPTR var, char *str1,char *);

typedef  struct  {
  int type;
  void   (*fonc) (FILE *f, VARPTR var, int flag ) ;} CheckRhsTab ;

extern CheckRhsTab CHECKTAB[];

