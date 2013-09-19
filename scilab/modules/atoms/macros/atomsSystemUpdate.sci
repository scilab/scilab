// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// End user function

// Installation of a toolbox

function atomsSystemUpdate()

    // Load Atoms Internals lib if it's not already loaded
    // =========================================================================
    if ~ exists("atomsinternalslib") then
        load("SCI/modules/atoms/macros/atoms_internals/lib");
    end

    if (atomsGetConfig("offLine") == "True" | atomsGetConfig("offline") == "True") then
        warning(msprintf(gettext("Option offline of ATOMS configuration is set to True. atomsSystemUpdate did not check the latest modules availables.")));
        return
    end

    // Check write access on allusers zone
    // =========================================================================
    atomsDESCRIPTIONget(%T);

endfunction
