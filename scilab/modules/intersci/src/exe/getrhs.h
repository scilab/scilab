/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2008 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

void GetMATRIX (FILE *f, VARPTR var, int flag);
void GetCom (FILE *f, VARPTR var, int flag);
void GetSTRING (FILE *f, VARPTR var, int flag);
void GetBMATRIX (FILE *f, VARPTR var, int flag);
void GetIMATRIX (FILE *f, VARPTR var, int flag);
void GetSPARSE (FILE *f, VARPTR var, int flag);
void GetSTRINGMAT (FILE *f, VARPTR var, int flag);
void GetROW (FILE *f, VARPTR var, int flag);
void GetCOLUMN (FILE *f, VARPTR var, int flag);
void GetVECTOR (FILE *f, VARPTR var, int flag);
void GetPOLYNOM (FILE *f, VARPTR var, int flag);
void GetSCALAR (FILE *f, VARPTR var, int flag);
void GetPOINTER (FILE *f, VARPTR var, int flag);
void GetANY (FILE *f, VARPTR var, int flag);
void GetLIST (FILE *f, VARPTR var, int flag );
void GetTLIST (FILE *f, VARPTR var, int flag);
void GetSEQUENCE (FILE *f, VARPTR var, int flag);
void GetEMPTY (FILE *f, VARPTR var, int flag);
void GetWORK (FILE *f, VARPTR var, int flag);
void GetDIMFOREXT (FILE *f, VARPTR var, int flag);
void Check (FILE *f, VARPTR var, int nel);
void CheckSquare (FILE *f, VARPTR var, char *str1,char *);

typedef  struct  {
  int type;
  void   (*fonc)(FILE *f, VARPTR var, int flag ) ;} GetRhsTab ;

extern GetRhsTab RHSTAB[];

