// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


//ex11 example
//1-Creating interface source file
// from ex11fi.desc file by call to intersci
// Making object files
//  Interface file '/tmp/ex11fi.o'
//  User's files '/tmp/ex11c.o';
files=G_make(["/tmp/ex11fi.o","/tmp/ex11c.o"],"ex11.dll");
//2-Link object files .o with addinter
//addinter(files,'intex11',intex1_funs);
exec("ex11fi.sce");
//Run Scilab functions:
a=[1,2,3]+%i*[4,5,6];
b=ext11c(a);
if norm(b-([4,5,6]+%i*[1,2,3])) > %eps then pause,end



