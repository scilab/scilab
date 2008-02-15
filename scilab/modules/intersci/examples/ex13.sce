// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


//ex13 example
//1-Creating interface source file
// Making object files
// Interface file '/tmp/ex13fi.o'
// User's files '/tmp/ex13c.o'
files=G_make(['/tmp/ex13fi.o','/tmp/ex13c.o'],'ex13.dll');
//2-Link object files .o with addinter
//addinter(files,'intex13',intex1_funs);
exec('ex13fi.sce');
//Run Scilab functions:
a=rand(2,3);
b=ext13c(a);
if norm(b- matrix(2*a,3,2)) > %eps then pause,end



