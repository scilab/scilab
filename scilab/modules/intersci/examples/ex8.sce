// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


//1-Creating interface source file (matusr.f)
// from ex8fi.desc file by call to intersci
// Making object files
//  Interface file '/tmp/ex8fi.o'
//  User's files '/tmp/ex8c.o';
files=G_make(['/tmp/ex8fi.o','/tmp/ex8c.o'],'ex8.dll');
//2-Link object files .o with addinter
//addinter(files,'ex8fi',matusr_funs);
exec('ex8fi.sce');
//Test Scilab functions:

//calc8: matrix of integer type created by C function (malloc and free).
a=calc8();
if norm(a - matrix(0:14,3,5)) > %eps then pause,end






