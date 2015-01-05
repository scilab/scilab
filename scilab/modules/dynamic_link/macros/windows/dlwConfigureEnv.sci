// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 2014 - Antoine ELIAS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

//=============================================================================
function dlwConfigureEnv()
    if getenv("SCILAB_MSVC", "0") == "0" then
        // Configure Ms VC environment
        configure_msvc();
        if findmsifortcompiler() <> "unknown" then
            // Configure Intel Fortran environment
            configure_msifort();
        else
            // Use F2C
            setenv("USE_F2C", "YES");
        end
    end
endfunction