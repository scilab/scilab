/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Sylvestre LEDRU
 * Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __OUTEXT_H__
#define __OUTEXT_H__

void OutExtCommon(FILE *f,VARPTR var,int insidelist,int nel);

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

#endif /* !__OUTEXT_H__ */
