// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


//ex1 example
//1-Creating interface source file
//  from ex1fi.desc file by call to intersci (see Makefile)
//  Making object files
files=G_make(["/tmp/ex1fi.o","/tmp/ex1c.o"],"ex1.dll");
//2-Link object files .o with addinter
//addinter(files,'intex1',intex1_funs);
exec("ex1fi.sce");
//Run Scilab functions:
a=[1,2,3];b=[4,5,6];
c=ext1c(a,b);
if norm(c-(a+b)) > %eps then pause,end


