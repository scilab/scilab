// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
// Copyright (C) DIGITEO - 2009-2010 - Allan CORNET
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
function bOK = configure_msvc()

    bOK = %F;
    if getos() <> "Windows" then
        return
    end

    // Load dynamic_link Internal lib if it's not already loaded
    if ~ exists("dynamic_linkwindowslib") then
        load("SCI/modules/dynamic_link/macros/windows/lib");
    end

    // Load dynamic_link Internal lib if it's not already loaded
    if ~ exists("dynamic_linkutilslib") then
        load("SCI/modules/dynamic_link/macros/utils/lib");
    end

    msvc = findmsvccompiler();
    if ~isempty(msvc) then
        val = setenv("SCILAB_MSVC", "1");
    end

    bOK = %T;
endfunction
//=============================================================================

