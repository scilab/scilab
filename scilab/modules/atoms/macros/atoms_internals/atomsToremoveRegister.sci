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

function nbAdd = atomsToremoveRegister(name,version,section)

    rhs            = argn(2);
    nbAdd          = 0;

    // Check number of input arguments
    // =========================================================================

    if rhs <> 3 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsToremoveRegister",3));
    end

    // Check input parameters type
    // =========================================================================

    if type(name) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsToremoveRegister",1));
    end

    if type(version) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsToremoveRegister",2));
    end

    // name and version must have the same size
    // =========================================================================

    if or( size(name) <> size(version) ) then
        error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),"atomsToremoveRegister",1,2));
    end

    // Allusers/user management
    // =========================================================================

    if type(section) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsToremoveRegister",3));
    end

    if and(section<>["user","allusers"]) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'' or ''allusers'' expected.\n"),"atomsToremoveRegister",3));
    end

    // Check if we have the write access
    if section=="allusers" & ~ atomsAUWriteAccess() then
        error(msprintf(gettext("%s: You haven''t write access on this directory : %s.\n"),"atomsToremoveRegister",3,atomsPath("system","allusers")));
    end

    // Define the path of the file that will record the change according to
    // the "section" value
    // =========================================================================
    atoms_directory = atomsPath("system",section);

    // Does the atoms_directory exist, if not create it
    // =========================================================================

    if ~ isdir(atoms_directory) then
        mkdir(atoms_directory);
    end

    // Define the path of the file that will record the change
    // =========================================================================

    toremove_bin = atoms_directory+"toremove.bin";

    // Get the toremove matrix
    // =========================================================================

    if fileinfo(toremove_bin) <> [] then
        load(toremove_bin,"toremove_mat");
    else
        toremove_mat = [];
    end

    // Loop on input arguments
    // =========================================================================

    for i=1:size(name,"*")
        // Add the module only if the module is not already present in the matrix
        if ~ or(toremove_mat == [name(i) version(i)]) then
            toremove_mat = [ toremove_mat ; name(i) version(i)  ];
            nbAdd        = nbAdd + 1;
        end
    end

    // Apply changes
    // =========================================================================

    if nbAdd > 0 then
        save(toremove_bin, "toremove_mat");
    end

endfunction
