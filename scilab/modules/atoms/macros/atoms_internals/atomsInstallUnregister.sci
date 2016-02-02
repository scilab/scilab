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

// Internal function

// Delete an atoms package from the list of available package
// This function has an impact on the following files :
//  -> ATOMSDIR/installed
//  -> ATOMSDIR/installed_deps

function atomsInstallUnregister(name,version,section)

    rhs = argn(2);

    // Check number of input arguments
    // =========================================================================

    if rhs <> 3 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsInstallUnregister",3));
    end

    // Check input parameters type
    // =========================================================================

    if type(name) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsInstallUnregister",1));
    end

    if type(version) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsInstallUnregister",2));
    end

    if type(section) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsInstallUnregister",2));
    end

    if and(section<>["user","allusers"]) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'' or ''allusers'' expected.\n"),"atomsInstallUnregister",2));
    end

    // name and version must have the same size
    // =========================================================================

    if or( size(name) <> size(version) ) then
        error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),"atomsInstallUnregister",1,2));
    end

    // Process installed
    // =========================================================================
    installed_before = atomsLoadInstalledStruct(section);
    installed_after  = atomsRmfields(installed_before,name+" - "+version);
    atomsSaveInstalled(installed_after,section);

    // Process installed dependencies
    // =========================================================================
    installed_deps_before = atomsLoadInstalleddeps(section);
    installed_deps_after  = atomsRmfields(installed_deps_before,name+" - "+version);
    atomsSaveInstalleddeps(installed_deps_after,section);

endfunction
