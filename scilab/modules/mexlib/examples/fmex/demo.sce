
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

// Allan CORNET
// INRIA 2007

exec(SCI+"/modules/mexlib/examples/fmex/builder.sce");
exec(SCI+"/modules/mexlib/examples/fmex/loader.sce");

[a,b]=mexf1(1,2);

if (b~=2) then
    disp("problem with this example");
else
    disp("ok");
end
