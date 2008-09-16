// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


files=['intex12c.c'];
ilib_build('gw_ex12c',['ex12c','intex12c'],files,[]);

exec loader.sce;

deff('p=mypoly(x)','p=poly(x,''z'',''coeffs'')')
c=[1,2,3];
P1=ex12c(2,c);

