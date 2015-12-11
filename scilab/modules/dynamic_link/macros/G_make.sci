// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
// Copyright (C) DIGITEO - 2010 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
