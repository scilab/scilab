/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/ 
#ifndef __CONVERTBASE_H__
#define __CONVERTBASE_H__

/**
* convert from base b to decimal
* @param string base
* @param int base
* @param[out] int if 0 no error  
* @return converted value (decimal)
*/
double convertBase2Dec(const char *pStr, int numberbase, int *ierr);

#endif /* __CONVERTBASE_H__ */
/*--------------------------------------------------------------------------*/ 
