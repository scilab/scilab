// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET , Sylvestre LEDRU
// Copyright (C) DIGITEO - 2010 - Allan CORNET
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
