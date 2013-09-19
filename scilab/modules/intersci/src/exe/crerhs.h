/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2008 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

void CreMATRIX(FILE *f, VARPTR var);
void CreCommon(FILE *f, VARPTR var);
void CreSTRING(FILE *f, VARPTR var);
void CreBMATRIX(FILE *f, VARPTR var);
void CreDIMFOREXT(FILE *f, VARPTR var);
void CreVECTOR(FILE *f, VARPTR var);
void CreCOLUMN(FILE *f, VARPTR var);
void CreSPARSE(FILE *f, VARPTR var);
void CreIMATRIX(FILE *f, VARPTR var);
void CrePOINTER(FILE *f, VARPTR var);
void CreSTRINGMAT(FILE *f, VARPTR var);
void CreSCALAR_old(FILE *f, VARPTR var);
void CreSCALAR(FILE *f, VARPTR var);
void CreANY(FILE *f, VARPTR var);
void CreEMPTY  (FILE *f, VARPTR var);


typedef  struct
{
    int type;
    void  (*fonc)(FILE *f, VARPTR var);
} CreRhsTab ;

extern CreRhsTab CRERHSTAB[];


