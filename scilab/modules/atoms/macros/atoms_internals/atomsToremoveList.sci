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

function toremove = atomsToremoveList(section)

    rhs        = argn(2);
    toremove   = [];

    // Check number of input arguments
    // =========================================================================

    if rhs > 1 then
        error(msprintf(gettext("%s: Wrong number of input argument: at most %d expected.\n"),"atomsToremoveList",1));
    end

    // Allusers/user management
    //   - If section is equal to "all" or to True, packages located in both
    //     "allusers" and "user" sections will removed.
    //   - If section is equal to "allusers", only packages located in the
    //     "allusers" section will be removed.
    //   - If section is equal to "user" or to False, only packages located in
    //     the "user" will be removed
    // =========================================================================

    if rhs <= 1 then

        // By default:
        //  → Remove packages located in both "allusers" and "user" sections if
        //    we have the write access to SCI directory
        //  → Remove only package located in the "user" sections otherwise

        if atomsAUWriteAccess() then
            section = "all";
        else
            section = "user";
        end

    else

        // Process the 2nd input argument : section
        // Allusers can be a boolean or equal to "user" or "allusers"

        if type(section) <> 10 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsToremoveList",1));
        end

        if and(section<>["user","allusers","all"]) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'' or ''allusers'' or ''all'' expected.\n"),"atomsToremoveList",1));
        end

    end

    // Define the path of the file that will record the change according to
    // the "section" value
    // =========================================================================
    atoms_directories = atomsPath("system",section);

    // Get the toremove matrix
    // =========================================================================

    for i=1:size(atoms_directories,"*")
        toremove_bin = atoms_directories(i)+"toremove.bin";
        if fileinfo(toremove_bin) <> [] then
            load(toremove_bin,"toremove_mat");
            toremove = [ toremove ; toremove_mat ];
        end
    end

endfunction
