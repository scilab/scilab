// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


//1-Creating interface source file (matusr.f)
// from ex9fi.desc file by call to intersci
// Making object files
//   Interface file '/tmp/ex9fi.o'
//   User's files '/tmp/ex9f.o';
files=G_make(["/tmp/ex9fi.o","/tmp/ex9f.o"],"ex9.dll");
//2-Link object files .o with addinter
//addinter(files,'ex9fi',matusr_funs);
exec("ex9fi.sce");
//Test Scilab functions:
//calc7: function with two optional values, a string and a scilab variable.
bb=33;  //scilab optional variable
[a,b,c,d]=calc9(10);
if c<>"toto" then pause,end
if norm(d-[2,4,6,8])> %eps then pause,end
if b<>bb then pause,end










