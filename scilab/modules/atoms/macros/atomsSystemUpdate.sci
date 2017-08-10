// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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
