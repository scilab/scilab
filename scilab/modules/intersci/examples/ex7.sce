// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


//ex7 example
//1-Creating interface source file
// from ex7fi.desc file by call to intersci
// Making object files
// Interface file '/tmp/ex7fi.o'
// User's files '/tmp/ex7f.o';
files=G_make(['/tmp/ex7fi.o','/tmp/ex7f.o'],'ex7.dll');
//2-Link object files .o with addinter
//addinter(files,'intex7',intex1_funs);
exec('ex7fi.sce');
//Run Scilab functions:
if calc('one')<>1 then pause,end
if calc('two')<>2 then pause,end
if calc('other')<>-1 then pause,end

