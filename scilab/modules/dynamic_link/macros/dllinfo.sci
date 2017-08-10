// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - Allan CORNET
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
function dllinfolist = dllinfo(dllname, options)

    dllinfolist = list();
    if getos() <> "Windows" then
        warning(msprintf(gettext("%s: This feature has been implemented for Windows.\n"),"dllinfo"));
    else
        // Load dynamic_link Internal lib if it's not already loaded
        if ~ exists("dynamic_linkwindowslib") then
            load("SCI/modules/dynamic_link/macros/windows/lib");
        end

        [lhs,rhs] = argn(0);

        if rhs == 1 then
            options = "exports";
        end

        if type(dllname) <> 10 | size(dllname, "*") <> 1  then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"dllinfo",1));
        end

        if fileinfo(dllname) == [] then
            error(msprintf(gettext("%s: The file %s does not exist.\n"),"dllinfo",dllname));
        end

        fext = fileext(dllname);
        if ~(strcmp(fext,".exe", "i") == 0 | strcmp(fext,".dll", "i")== 0) then
            error(msprintf(gettext("%s: Cannot open file %s.\n"),"dllinfo",dllname));
        end

        if type(options) <> 10 | size(options) <> 1 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"dllinfo",2));
        end

        if ~(strcmp(options,"imports","i")== 0 | strcmp(options,"exports", "i")== 0 | strcmp(options, "machine", "i")== 0) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),"dllinfo",2,"''imports'',''exports'',''machine''"));
        end

        dllinfolist = dlwDllInfo(dllname, options);
    end
endfunction
//=============================================================================
