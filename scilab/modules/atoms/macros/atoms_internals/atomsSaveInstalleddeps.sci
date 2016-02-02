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

// Save two trees :
//   - package -> childs : give the list of packages needed by "package"
//   - package -> parents : give the list of packages used by "package"

// The two trees are saved in two formats :
//   - text file   : installed_deps.txt
//   - binary file : installed_deps.bin


// Input arguments :

//   child_deps : .list of packages needed by "package"
//                . struct
//                . mandatory

//   allusers : . Tell where will be record the change
//              . boolean
//              . optional

function  atomsSaveInstalleddeps(child_deps,section)

    rhs = argn(2);

    // Check number of input arguments
    // =========================================================================

    if rhs <> 2 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsSaveInstalleddeps",2));
    end

    // Check type of input argument type
    // =========================================================================

    if type(child_deps) <> 17 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Struct expected.\n"),"atomsSaveInstalleddeps",1));
    end

    if type(section) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsSaveInstalleddeps",2));
    end

    if size(section,"*")<>1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: string expected.\n"),"atomsSaveInstalleddeps",2));
    end

    if and(section<>["user","allusers"]) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'' or ''allusers'' expected.\n"),"atomsSaveInstalleddeps",2));
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

    // Define the path of the file that will record the change according to
    // =========================================================================
    installed_deps_txt = atoms_directory+"installed_deps.txt";
    installed_deps_bin = atoms_directory+"installed_deps.bin";


    // List of installed packages
    // =========================================================================
    parents_packages = getfield(1,child_deps);
    parents_packages(1:2) = [];


    // Build the package->parents struct
    // =========================================================================

    parent_deps = struct();

    for i=1:size(parents_packages,"*")

        this_package_parents = [];

        for j=1:size(parents_packages,"*")

            child_packages = child_deps( parents_packages(j) );
            if find(parents_packages(i) == child_packages) <> [] then
                this_package_parents = [ this_package_parents ;  parents_packages(j) ];
            end

        end

        parent_deps( parents_packages(i) ) = this_package_parents;

    end

    // Build the string matrix (=> text file)
    // =========================================================================

    string_matrix = [];

    // childs
    // -------------------------------------------------------------------------

    string_matrix = [ string_matrix ; "# ==============================================================================" ];
    string_matrix = [ string_matrix ; "# Packages childs" ];
    string_matrix = [ string_matrix ; "# ==============================================================================" ];
    string_matrix = [ string_matrix ; "" ];

    for i=1:size(parents_packages,"*")

        // parent
        string_matrix = [ string_matrix ; "["+parents_packages(i)+"]" ];

        // childs
        child_package = child_deps( parents_packages(i) );
        for j=1:size(child_package,"*")
            string_matrix = [ string_matrix ; "    "+child_package(j) ];
        end

        // spaces
        string_matrix = [ string_matrix ; "" ];

    end

    // parents
    // -------------------------------------------------------------------------

    string_matrix = [ string_matrix ; "" ];
    string_matrix = [ string_matrix ; "# ==============================================================================" ];
    string_matrix = [ string_matrix ; "# Packages parents" ];
    string_matrix = [ string_matrix ; "# ==============================================================================" ];
    string_matrix = [ string_matrix ; "" ];

    for i=1:size(parents_packages,"*")

        // parent
        string_matrix = [ string_matrix ; "["+parents_packages(i)+"]" ];

        // childs
        parent_package = parent_deps( parents_packages(i) );
        for j=1:size(parent_package,"*")
            string_matrix = [ string_matrix ; "    "+parent_package(j) ];
        end

        // spaces
        string_matrix = [ string_matrix ; "" ];

    end

    // If parents_packages is empty, no need to keep the files
    // =========================================================================

    if isempty(parents_packages) then
        mdelete(installed_deps_txt);
        mdelete(installed_deps_bin);
        return;
    end

    // write files
    // =========================================================================

    mputl(string_matrix,installed_deps_txt);
    save(installed_deps_bin, "child_deps", "parent_deps");

endfunction
