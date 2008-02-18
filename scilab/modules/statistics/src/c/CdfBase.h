
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
 
#ifndef __CDFBASE_H__
#define __CDFBASE_H__

/**
 * TODO : needs comments
 * @param fname
 * @param inarg
 * @param oarg
 * @param callpos
 * @param option
 * @param errnames
 * @param which
 * @param (*fonc)()
 * @param (*foncErr)()
 * @return
 */

int  CdfBase(char *fname,int inarg,int oarg,int *callpos,char *option,char *errnames,int which,int (*fonc)(),void (*foncErr)() );

#endif /* __CDFBASE_H__ */
