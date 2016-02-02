// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007 - INRIA - Allan CORNET
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// start a command PowerShell
function [resultat, status] = powershell(varargin)
    Chainecmd = "";
    Chainecmdbegin = "powershell.exe -nologo -inputformat text -outputformat text -Noninteractive ";
    resultat = [gettext("Error(s) : see help powershell");gettext("Please verify your powershell command without Scilab.")];
    status = %f;

    [lhs, rhs] = argn(0);

    if (getos() <> "Windows") then
        resultat = [gettext("Only for Microsoft Windows.")];
        return;
    end

    try
        winqueryreg("HKEY_LOCAL_MACHINE", "SOFTWARE\Microsoft\PowerShell\1", "Install");
    catch
        resultat = [gettext("Powershell 1.0 not found.")];
        return;
    end

    if (rhs == 1) then
        if ( (type(varargin(1)) == 10) & and(size(varargin(1)) == [1 1]) ) then
            Chainecmd = Chainecmdbegin + "-command """ + varargin(1) + """";
            [resultat, status] = dos(Chainecmd);
        else
            error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"), "powershell", 1));
        end
    else
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "powershell", 1));
    end
endfunction
