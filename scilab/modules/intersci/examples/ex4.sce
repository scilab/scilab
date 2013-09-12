// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


//ex4 example
//1-Creating interface source file
// from intex4.desc file by call to intersci (see Makefile)
// Making object files
//   Interface file '/tmp/ex4fi.o'
//   User's files '/tmp/ex4f.o';
files=G_make(["/tmp/ex4fi.o","/tmp/ex4f.o"],"ex4.dll");
//2-Link object files .o with addinter
//addinter(files,'intex4',intex4_funs);
exec("ex4fi.sce");
//Run Scilab functions:
sys=ssrand(2,2,3,list("co",1));
n=contrb(sys,0.01)
if n <> 1 then pause,end




