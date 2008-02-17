// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


// function definition
deff('[x]=t4(a,n)',[
'[lhs,rhs]=argn(0),'
'[m,n]=size(a),'
'if m==n then error(''incorrect dimensions ''),end,'
'x=sin(a),'
'u=norm(x),'
'if u==%eps then x=x/u,end,'],'n')

// define type and dimension of the input args
lt4=list();
lt4(1)=list('1','m1','n1');

// show the initial data
printspecs(t4,lt4)

//translate
txt=sci2for(t4,'t4',lt4);
message(['Generated Fortran code';' ';txt])
