
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// Allan CORNET
// INRIA 2007

cd "SCI/modules/mexlib/examples/mexdll"

if (findmsvccompiler() <> "unknown") then
    unix_w("lib /def:xtimesy.def");
    unix_w("nmake /f makelib.mak all");

    addinter("libtst.dll","libtst","xtimesy")
    xtimesy(2,3)

end
