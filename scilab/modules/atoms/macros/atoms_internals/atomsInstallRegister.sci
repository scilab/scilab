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

// Add toolboxes to the list of installed packages
// This function has an impact on the following files :
//  -> ATOMSDIR/installed.txt, ATOMSDIR/installed.bin
//  -> ATOMSDIR/installed_deps.txt, ATOMSDIR/installed_deps.bin

function nbAdd = atomsInstallRegister(name,version,status,section)

    rhs            = argn(2);
    nbAdd          = 0;

    // Check number of input arguments
    // =========================================================================

    if rhs <> 4 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsInstallRegister",3,4));
    end

    // Check input parameters type
    // =========================================================================

    if type(name) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsInstallRegister",1));
    end

    if type(version) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsInstallRegister",2));
    end

    if type(status) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsInstallRegister",3));
    end

    if type(section) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsInstallRegister",4));
    end

    // status is a letter
    // =========================================================================

    if ~ and((status == "A") | (status == "I"))  then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Letters ''A'' or ''I'' expected.\n"),"atomsInstallRegister",3));
    end

    // name,version and status must have the same size
    // =========================================================================

    if or( size(name) <> size(version) ) then
        error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),"atomsInstallRegister",1,2));
    end

    if or( size(name) <> size(status) ) then
        error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),"atomsInstallRegister",1,3));
    end

    // Check section argument
    // =========================================================================

    if and(section<>["user","allusers"]) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'' or ''allusers'' expected.\n"),"atomsInstallRegister",4));
    end

    // Check if we have the write access
    if (section=="allusers") & ~ atomsAUWriteAccess() then
        error(msprintf(gettext("%s: You haven''t write access on this directory : %s.\n"),"atomsInstallRegister",2,atomsPath("system","allusers")));
    end

    // load installed packages (a struct)
    // =========================================================================
    installed = atomsLoadInstalledStruct(section);

    // Load the installed_deps (a struct)
    // =========================================================================
    installed_deps = atomsLoadInstalleddeps(section);

    // Loop on each URL specified as input argument
    // =========================================================================
    for i=1:size(name,"*")

        if isfield(installed,name(i)+" - "+version(i)) then
            // This package is already registered
            continue;
        else
            // We have at least one change
            nbAdd = nbAdd + 1;
        end

        // installed struct
        // ---------------------------------------------------------------------

        // Build the matrix
        this_package_path = pathconvert(atomsPath("install",section) + name(i)+"/"+version(i),%F);

        // Add this package to the struct
        installed(name(i)+" - "+version(i)) = [ ..
        name(i)              ; .. // name
        version(i)           ; .. // version
        this_package_path    ; .. // path
        section              ; .. // allusers / user
        status(i)            ];   // I / A

        // installed_deps file
        // Get the depencency tree & and child package
        // ---------------------------------------------------------------------

        childs_tree = atomsDepTreeFlat(name(i),version(i));
        childs_mat  = getfield(1,childs_tree);
        childs_mat(1:2) = [];
        childs_mat( find(childs_mat == name(i)+" - "+version(i)) ) = [];
        installed_deps(name(i)+" - "+version(i)) = childs_mat';
    end

    // Apply changes
    // =========================================================================

    if nbAdd > 0 then
        atomsSaveInstalled(installed,section);
        atomsSaveInstalleddeps(installed_deps,section);
    end

endfunction
