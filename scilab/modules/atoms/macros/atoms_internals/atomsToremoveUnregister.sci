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

// Add toolboxes to the list of packages to remove
// This function has an impact on the following files :
//  -> ATOMSDIR/toremove.bin

function atomsToremoveUnregister(name,version,section)

    rhs = argn(2);

    // Check number of input arguments
    // =========================================================================

    if rhs <> 3 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsToremoveUnregister",3));
    end

    // Check input parameters type
    // =========================================================================

    if type(name) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsToremoveUnregister",1));
    end

    if type(version) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsToremoveUnregister",2));
    end

    // Allusers/user management
    // =========================================================================

    if type(section) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsToremoveUnregister",3));
    end

    if and(section<>["user","allusers"]) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'' or ''allusers'' expected.\n"),"atomsToremoveUnregister",3));
    end

    // Check if we have the write access
    if section=="allusers" & ~ atomsAUWriteAccess() then
        error(msprintf(gettext("%s: You haven''t write access on this directory : %s.\n"),"atomsToremoveUnregister",3,atomsPath("system","allusers")));
    end

    // name and version must have the same size
    // =========================================================================

    if or( size(name) <> size(version) ) then
        error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),"atomsToremoveUnregister",1,2));
    end

    // Define the path of the file that will record the change
    // =========================================================================

    toremove_bin = atomsPath("system",section)+"toremove.bin";

    // Get the toremove matrix
    // =========================================================================

    if fileinfo(toremove_bin) <> [] then
        load(toremove_bin,"toremove_mat");
    else
        return;
    end

    // Loop on input arguments
    // =========================================================================
    for i=1:size(name,"*")
        toremove_mat(toremove_mat == [name(i) version(i)]) = [];
    end

    // Apply changes
    // =========================================================================
    if isempty(toremove_mat) then
        save(toremove_bin, "toremove_mat");
    else
        mdelete(toremove_bin);
    end

endfunction
