// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 2014 - Antoine ELIAS
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