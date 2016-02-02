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

// Add a package description to a DESCRIPTION struct
// The DESCRIPTION struct looks like that

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

function tree_out = atomsDESCRIPTIONadd( tree_in , package_name , package_version , description )

    // Check input parameters number
    // =========================================================================

    rhs  = argn(2);

    if rhs <> 4 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsDESCRIPTIONadd",4));
    end

    // Check input parameters type
    // =========================================================================

    if type(tree_in) <> 17 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Struct expected.\n"),"atomsDESCRIPTIONadd",1));
    end

    if type(package_name) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsDESCRIPTIONadd",2));
    end

    if type(package_version) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsDESCRIPTIONadd",3));
    end

    if type(description) <> 17 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Struct expected.\n"),"atomsDESCRIPTIONadd",4));
    end

    // Check input parameters size
    // =========================================================================

    if size(package_name,"*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: string expected.\n"),"atomsDESCRIPTIONadd",2));
    end

    if size(package_version,"*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: string expected.\n"),"atomsDESCRIPTIONadd",3));
    end

    // And now ... action
    // =========================================================================

    tree_out     = tree_in;

    if isfield(tree_out,"packages") then
        packages_out = tree_out("packages");
    else
        packages_out = struct();
    end

    if isfield(packages_out,package_name) then
        package_name_struct = packages_out(package_name);
    else
        package_name_struct = struct();
    end

    package_name_struct(package_version) = description;
    packages_out(package_name)           = package_name_struct;
    tree_out("packages")                 = packages_out;

endfunction
