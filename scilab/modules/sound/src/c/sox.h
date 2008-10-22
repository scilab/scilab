/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef SCI_SOX 
#define SCI_SOX 

#include "wav.h" 
#include "machine.h"

int C2F(playsound)(char *filename,char *command,unsigned long fname_len);
int C2F(loadwave)(char *filename, double *res, int *size_res, int flag,WavInfo *, int *ierr);
int C2F(savewave)(char *filename, double *res, int *rate, int *size_res,int *nchannels, int *ierr);
#endif
