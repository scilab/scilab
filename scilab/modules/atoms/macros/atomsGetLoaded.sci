// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
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

// Return a 4xn string matrix :
//
// !toolbox2          0.1  user     SCIHOME/atoms/contrib/toolbox2/0.1  A  !
// !toolbox2          0.2  user     SCIHOME/atoms/contrib/toolbox2/0.2  A  !
// !toolbox_skeleton  1.3  alluser  SCI/contrib/toolbox_skeleton/1.3    I  !

function varargout = atomsGetLoaded()

    // Load Atoms Internals lib if it's not already loaded
    // =========================================================================
    if ~ exists("atomsinternalslib") then
        load("SCI/modules/atoms/macros/atoms_internals/lib");
    end

    loaded = atomsLoadLoad();

    if isempty(loaded) then
        packages = [];
    else
        packages = atomsGetInstalledDetails(loaded);
    end

    // Set the result
    // =========================================================================
    if argn(1) > 0 then
        varargout = list(packages)
    else
        mprintf("%s\n", strcat(justify(packages,"l"), "  ", "c"))
    end

endfunction
