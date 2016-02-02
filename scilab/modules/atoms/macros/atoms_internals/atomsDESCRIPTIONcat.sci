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

// Merge two descriptions structures ( created by atomsDESCRIPTIONread() )
//
// DESCRIPTION
// |
// |-- packages                              [1x1 struct]
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
// |-- categories                            [1x1 struct]
// |   |-- Optimization                      [1x1 struct]
// |   |   |-- Linear
// |   |   `-- General
// |   `-- ..
// |
// |-- categories_flat                       [1x1 struct]
//     |-- Optimization - Linear
//     |   `-- [ "Optimization" ; "Linear" ]
//     |-- Optimization - General
//     `-- Optimization - General
//

function tree_out = atomsDESCRIPTIONcat( tree_in_1 , tree_in_2 )

    rhs = argn(2);

    // Check number of input arguments
    // =========================================================================

    if rhs <> 2 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsDESCRIPTIONcat",1));
    end

    // Check input arguments type
    // =========================================================================

    if type(tree_in_1) <> 17 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: matrix oriented typed list expected.\n"),"atomsDESCRIPTIONcat",1));
    end

    if type(tree_in_2) <> 17 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: matrix oriented typed list expected.\n"),"atomsDESCRIPTIONcat",2));
    end

    // Check the presence of needed fields in the first input argument
    // =========================================================================

    needed_fields = ["packages";"categories";"categories_flat"];

    for i=1:size(needed_fields,"*")
        if ~ isfield(tree_in_2,needed_fields(i)) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: It should have a field named ''%s''.\n"),"atomsDESCRIPTIONcat",2,needed_fields(i)));
        end
    end

    // tree_in_1 is the master tree :
    // =========================================================================

    tree_out  = tree_in_1;

    // Fill needed fields with empty structs if they don't exist

    for i=1:size(needed_fields,"*")
        if ~ isfield(tree_out,needed_fields(i)) then
            tree_out(needed_fields(i)) = struct();
        end
    end

    // Now do merges
    // =========================================================================

    // packages
    // -------------------------------------------------------------------------

    packages_out           = tree_out("packages");
    packages_in_2          = tree_in_2("packages");
    packages_out           = atomsPackagesCat(packages_out,packages_in_2);
    tree_out("packages")   = packages_out;


    // categories
    // -------------------------------------------------------------------------

    categories_out         = tree_out("categories");
    categories_in_2        = tree_in_2("categories");
    categories_out         = atomsCategoriesCat(categories_out,categories_in_2);
    tree_out("categories") = categories_out;


    // categories_flat
    // -------------------------------------------------------------------------

    categories_flat_out  = tree_out("categories_flat");
    categories_flat_in_2 = tree_in_2("categories_flat");
    category_names       = getfield(1,categories_flat_in_2);
    category_names(1:2)  = [];

    for i=1:size(category_names,"*")
        if ~ isfield(categories_flat_out,category_names(i)) then
            categories_flat_out(category_names(i)) = categories_flat_in_2(category_names(i));
        end
    end

    tree_out("categories_flat") = categories_flat_out;

endfunction

// =============================================================================
// atomsCategoriesCat
// =============================================================================

function tree_out = atomsCategoriesCat( tree_in_1 , tree_in_2 )

    rhs = argn(2);

    // Check number of input arguments
    // =========================================================================

    if rhs <> 2 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsCategoriesCat",1));
    end

    // Check input arguments type
    // =========================================================================

    if type(tree_in_1) <> 17 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: matrix oriented typed list expected.\n"),"atomsCategoriesCat",1));
    end

    if type(tree_in_2) <> 17 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: matrix oriented typed list expected.\n"),"atomsCategoriesCat",2));
    end

    // tree_in_1 is the master tree :
    // =========================================================================

    tree_out  = tree_in_1;

    // Loop on tree_in_2 fields
    // =========================================================================

    category_names      = getfield(1,tree_in_2);
    category_names(1:2) = [];

    for i=1:size(category_names,"*")

        if isfield(tree_out,category_names(i)) then

            // The category already exists in tree_in_1

            subcategories_2   = tree_in_2(category_names(i));
            subcategories_out = tree_out(category_names(i));

            for j=1:size(subcategories_2,"*")
                if find( subcategories_2(j) == subcategories_out ) == [] then
                    subcategories_out = [ subcategories_out ; subcategories_2(j) ];
                end
            end

            tree_out(category_names(i)) = subcategories_out;
        else
            // The category doesn't exist in tree_in_1
            tree_out(category_names(i)) = tree_in_2(category_names(i));
        end

    end

endfunction

// =============================================================================
// atomsPackagesCat
// =============================================================================

function tree_out = atomsPackagesCat( tree_in_1 , tree_in_2 )

    rhs = argn(2);

    // Check number of input arguments
    // =========================================================================

    if rhs <> 2 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsPackagesCat",1));
    end

    // Check input arguments type
    // =========================================================================

    if type(tree_in_1) <> 17 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: matrix oriented typed list expected.\n"),"atomsPackagesCat",1));
    end

    if type(tree_in_2) <> 17 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: matrix oriented typed list expected.\n"),"atomsPackagesCat",2));
    end

    // tree_in_1 is the master tree :
    // =========================================================================

    tree_out  = tree_in_1;

    // Loop on tree_in_2 fields
    // =========================================================================

    package_names      = getfield(1,tree_in_2);
    package_names(1:2) = [];

    for i=1:size(package_names,"*")

        if isfield(tree_out,package_names(i)) then

            // The package already exists in tree_in_1
            package_versions_struct = tree_in_2(package_names(i));

            if type(package_versions_struct) <> 17 then
                error(msprintf(gettext("%s: Wrong value for input argument #%d: The matrix oriented typed list is not well formatted.\n"),"atomsPackagesCat",1));
            end

            package_versions      = getfield(1,package_versions_struct);
            package_versions(1:2) = [];
            package_versions_out  = tree_out(package_names(i));

            for j=1:size(package_versions,"*")
                package_versions_out(package_versions(j)) = package_versions_struct(package_versions(j));
            end

            tree_out(package_names(i)) = package_versions_out;

        else
            // The package doesn't exist in tree_in_1
            tree_out(package_names(i)) = tree_in_2(package_names(i));
        end

    end

endfunction
