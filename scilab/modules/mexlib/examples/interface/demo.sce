
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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
