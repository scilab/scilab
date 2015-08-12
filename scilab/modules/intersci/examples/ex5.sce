// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


//foobar example
//1-Creating interface source file
// from ex5fi.desc file by call to intersci (see Makefile)
//2-Making object files
// Interface file '/tmp/ex5fi.o'
// User's files '/tmp/ex5f.o';
files=G_make(["/tmp/ex5fi.o","/tmp/ex5f.o"],"ex5.dll");
//2-Link object files .o with addinter
//addinter(files,'foobar',ex5fi_funs);
exec("ex5fi.sce");
//Run Scilab functions:
a=1:5;b=-a;c=ones(3,3);
[a,b,c,d]=foobar("mul",a,b,c)
[a,b,c,d]=foobar("add",a,b,c)


