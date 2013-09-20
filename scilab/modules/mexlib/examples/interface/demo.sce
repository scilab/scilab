
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

currentdir=pwd();
cd SCI+"/modules/mexlib/examples/interface/lib"

exec("builder.sce");

cd SCI+"/modules/mexlib/examples/interface"
exec("builder.sce");

exec(SCI+"/modules/mexlib/examples/interface/loader.sce");


if f1(89)<>89+2 then pause,end
if f2(89)<>89+3 then pause,end
if f3(89)<>89 then pause,end
if f4(89)<>89+5 then pause,end

cd currentdir
