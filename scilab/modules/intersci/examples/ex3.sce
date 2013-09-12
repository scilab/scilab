// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


//daxpy example
//1-Creating interface source file
// from ex3fi.desc file by call to intersci (see Makefile)
// Making object files
//   Interface file '/tmp/ex3fi.o'
//   User's files [];
files=G_make(["/tmp/ex3fi.o"],"ex3.dll");
//2-Link object files .o with addinter
//addinter(files,'daxpy',ex3fi_funs);
exec("ex3fi.sce");
//Run Scilab functions:
n=3;a=13;incx=2;incy=3;x=1:10;y=2*x;
ynew=scilabdaxpy(n,a,x,incx,y,incy);
y(1:incy:n*incy)=y(1:incy:n*incy)+a*x(1:incx:n*incx);
if norm(ynew-y) > %eps then pause,end



