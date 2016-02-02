// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
// Copyright (C) DIGITEO - 2010 - Allan CORNET
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
function res = G_make(files, objects_or_dll)

    if ~haveacompiler() then
        error(msprintf(gettext("%s: A Fortran or C compiler is required.\n"),"G_make"))
    end

    [lhs,rhs] = argn(0);
    if rhs <> 2 then
        error(msprintf(gettext("%s: Wrong number of input argument(s).\n"),"G_make"));
        return
    end

    msg = "";

    if getos() == "Windows" then // WINDOWS
        // Load dynamic_link Internal lib if it's not already loaded
        if ~ exists("dynamic_linkwindowslib") then
            load("SCI/modules/dynamic_link/macros/windows/lib");
        end
        res = dlwMake(files, objects_or_dll);
    else // LINUX

        mk = [];
        for x = files(:)', if strindex(x,"-l")==[], mk=mk+" "+x ; end ;end

        if ilib_verbose() > 1 then
            msg = unix_g("make "+ mk);
        else
            host("make "+ mk);
        end

        res = files ;

    end

    if ilib_verbose() > 1 then
        disp(msg);
    end

endfunction
//=============================================================================
