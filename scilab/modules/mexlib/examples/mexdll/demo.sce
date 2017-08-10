
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

cd "SCI/modules/mexlib/examples/mexdll"

if (findmsvccompiler() <> "unknown") then
    unix_w("lib /def:xtimesy.def");
    unix_w("nmake /f makelib.mak all");

    addinter("libtst.dll","libtst","xtimesy")
    xtimesy(2,3)

end
