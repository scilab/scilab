// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


//ex2 example
//1-Creating interface source file
// from intex2.desc file by call to intersci (see Makefile)
//  Making object files
//  Interface file '/tmp/ex2fi.o'
//  User's files ['/tmp/ex2f.o']
files=G_make(['/tmp/ex2fi.o','/tmp/ex2f.o'],'ex2.dll');
//2-Link object files .o with addinter
//addinter(files,'intex2',intex2_funs);
exec('ex2fi.sce');
//Run Scilab functions:
a=1:20;b=1:10;
d=som(a=1:20,b=1:10);
d1=ones(1,10) + a(1:10)+b(1:10);
if norm(d-d1') > %eps then pause,end;



