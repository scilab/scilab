// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


// function definition
deff('[x]=t5(a,b)','x=a+%i*b','n')

// define type and dimension of the input args
lt5=list();
lt5(1)=list('1','m1','n1',0);
lt5(2)=list('1','m1','n1',0);

// show the initial data
printspecs(t5,lt5)

//translate
txt=sci2for(t5,'t5',lt5);
message(['Generated Fortran code';' ';txt])
