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


