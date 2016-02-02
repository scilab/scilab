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

// Remove a package defined by its name and version from a DESCRIPTION struct
// The description struct looks like that :

// DESCRIPTION
// |
// |-- packages
// |   |-- toolbox_1                         [1x1 struct]
// |   |   |-- 2.0                           [1x1 struct]
// |   |   |   |-- Toolbox: "toolbox_2"
// |   |   |   |-- Title: "Toolbox Test 2"
// |   |   |   |-- Version: "2.0"
// |   |   |   `-- ..
// |   |   `-- 1.0                           [1x1 struct]
// |   |   |   |-- Toolbox: "toolbox_2"
// |   |   |   |-- Title: "Toolbox Test 2"
// |   |   |   |-- Version: "1.0"
// |   |   |   `-- ..
// |   |-- module_lycee
// |   `-- ..
// |
// |-- categories
// |-- categories_flat

function tree_out = atomsDESCRIPTIONrm( tree_in , package_name , package_version )

    // Check input parameters number
    // =========================================================================

    rhs  = argn(2);

    if rhs <> 3 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsDESCRIPTIONrm",3));
    end

    // Check input parameters type
    // =========================================================================

    if type(tree_in) <> 17 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Struct expected.\n"),"atomsDESCRIPTIONrm",1));
    end

    if type(package_name) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsDESCRIPTIONrm",2));
    end

    if type(package_version) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsDESCRIPTIONrm",3));
    end

    // Check input parameters size
    // =========================================================================

    if size(package_name,"*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: string expected.\n"),"atomsDESCRIPTIONrm",2));
    end

    if size(package_version,"*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: string expected.\n"),"atomsDESCRIPTIONrm",3));
    end

    // And now ... action
    // =========================================================================

    tree_out     = struct();
    packages_out = struct();

    if isfield(tree_in,"packages") then
        packages_in = tree_in("packages");
    else
        return;
    end

    package_names      = getfield(1,packages_in);
    package_names(1:2) = [];

    for i=1:size(package_names,"*")

        if package_names(i) <> package_name then
            packages_out(package_names(i)) = packages_in(package_names(i));
            continue;
        end

        package_versions_out  = struct();
        package_versions_in   = packages_in(package_names(i));
        package_versions      = getfield(1,package_versions_in);
        package_versions(1:2) = [];
        package_versions_size = size(package_versions,"*");

        if (package_versions_size == 1) & (package_versions == package_version) then
            continue;
        end

        for j=1:package_versions_size
            if package_versions(j) <> package_version then
                package_versions_out(package_versions(j)) = package_versions_in(package_versions(j));
            else
                is_present = %T;
            end
        end

        packages_out(package_names(i)) = package_versions_out;

    end

    tree_out("packages") = packages_out;

endfunction
