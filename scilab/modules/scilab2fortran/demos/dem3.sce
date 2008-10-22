// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


// function definition
deff('[x]=t3(a,b,c,d)','x=[a;c]/[a b;c d]*3.5','n')

// define type and dimension of the input args
lt3=list();
lt3(1)=list('1','m1','n1');
lt3(2)=list('1','m1','n2');
lt3(3)=list('1','m2','n1');
lt3(4)=list('1','m2','n2');

// show the initial data
printspecs(t3,lt3)

//translate
txt=sci2for(t3,'t3',lt3);
message(txt)
