// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 200X-2010 - Allan CORNET
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
function ifortCompiler = findmsifortcompiler()

    ifortCompiler = "unknown"; // unknown

    if getos() == "Windows" then
        if getenv("IFORT_COMPILER14","NOK") <> "NOK" then
            ifortCompiler = "ifort14";
            return
        end

        if getenv("IFORT_COMPILER13","NOK") <> "NOK" then
            ifortCompiler = "ifort13";
            return
        end

        if getenv("IFORT_COMPILER12","NOK") <> "NOK" then
            ifortCompiler = "ifort12";
            return
        end

        if getenv("IFORT_COMPILER11","NOK") <> "NOK" then
            ifortCompiler = "ifort11";
            return
        end

        if getenv("IFORT_COMPILER10","NOK") <> "NOK" then
            ifortCompiler = "ifort10";
            return
        end

        if getenv("IFORT_COMPILER9","NOK") <> "NOK" then
            ifortCompiler = "ifort9";
            return
        end
    else // NOT Windows
        ifortCompiler = "unknown"; // unknown
    end
endfunction
//=============================================================================


