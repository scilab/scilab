// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET , Sylvestre LEDRU
// Copyright (C) DIGITEO - 2010 - Allan CORNET
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

//=============================================================================
function bOK = haveacompiler()
    if getos() == "Windows" then
        // Load dynamic_link Internal lib if it's not already loaded
        if ~ exists("dynamic_linkwindowslib") then
            load("SCI/modules/dynamic_link/macros/windows/lib");
        end
        bOK = dlwHaveCompiler();
    else
        // Very hard to detect under Linux/Unix since there are plenty of
        // compiler... Then, we return all the time TRUE
        // The actual detection is done by the dedicated ./configure
        if ( ilib_verbose() == 2 ) then
            warning(msprintf(gettext("Due to the multivendors nature of the Unix world, the function ''haveacompiler'' is only supported under Windows.")));
        end
        bOK = %T;
    end
endfunction
//=============================================================================
