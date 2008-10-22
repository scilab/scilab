// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


//ex12 example
//1-Creating interface source file
// Making object files
// Interface file '/tmp/ex12fi.o'
// User's files '/tmp/ex12c.o';
files=G_make(['/tmp/ex12fi.o','/tmp/ex12c.o'],'ex12.dll');
//2-Link object files .o with addinter
//addinter(files,'intex12',intex1_funs);
exec('ex12fi.sce');
//Run Scilab functions:
b=ccalc12();

if norm(b-(0:9)) > %eps then pause,end




