// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 2017  - Antoine ELIAS
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

//=============================================================================
function cmd = dlwGetEnvCmd()
    if win64() then
        if dlwIsExpress() then
            arch = "x86_amd64";
        else
            arch = "x64";
        end
    else
        arch = "x86";
    end

    path = dlwGetVisualStudioPath();
    cmd = """" + path + "\vcvarsall.bat"" " + arch;
endfunction
