// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
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

// Input parameters
//
// - category: [], 1x1 or 1x2 string array
//   + []
//   + "Data Analysis And Statistics"
//   + "Data Analysis And Statistics - Neural networks"
//   + ["Data Analysis And Statistics","Neural networks"]
//
// - nameonly: %T or %F (%F is the default value)


// Returns the list of available packages
// - In ascending (lexical) order for the name
// - In descending order for the versions

// if nameonly is TRUE, a mx1 matrix is returned:
//
//         !ampl_toolbox      !
//         !                  !
//         !ANN_Toolbox       !
//         !                  !
//         !celestlab         !
//         !                  !
//         ...

// if nameonly is FALSE, a mx2 matrix is returned:
//
//        !ampl_toolbox      1.3-1      !
//        !                             !
//        !ampl_toolbox      1.2-1      !
//        !                             !
//        !ANN_Toolbox       0.4.2.3-1  !
//        !                             !
//        !celestlab         2.0.0-1    !
//        !                             !
//        !celestlab         1.0.1-1    !
//        ...

function packages = atomsGetAvailable(category,nameonly)

    packages = [];

    // Check input parameters
    // =========================================================================

    rhs = argn(2);

    if rhs > 2 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"atomsGetAvailable",0,2))
    end

    if rhs==0 then
        category = [];
    end

    if category<>[] then

        if type(category) <> 10 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsGetAvailable",1));
        end

        if (size(category,"*") <> 1) & or(size(category) <> [1 2]) then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: A 1x1 or 1x2 string matrix expected.\n"),"atomsGetAvailable",1));
        end

        if and(size(category)==[1 2]) then
            category = category(1)+" - "+category(2);
        end

        if ~atomsIsCategory(category) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: A valid category expected.\n"),"atomsGetAvailable",1));
        end

    end

    if rhs>=2 then

        if type(nameonly) <> 4 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: A boolean expected.\n"),"atomsGetAvailable",2));
        end

        if size(nameonly,"*") <> 1 then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: A boolean expected.\n"),"atomsGetAvailable",2));
        end

    else
        nameonly = %F;
    end

    // Get the list of available packages
    // =========================================================================

    [packages_struct,categories_struct] = atomsDESCRIPTIONget();

    if category==[] then
        // All packages
        packages_list      = getfield(1,packages_struct);
        packages_list(1:2) = [];
        packages_list      = packages_list';
    else
        // Filter by category
        category_struct    = categories_struct(category);
        packages_list      = category_struct("packages");
        packages_list      = unique(packages_list(:,1));
    end

    [A,k]         = gsort(convstr(packages_list,"l"),"lr","i");
    packages_list = packages_list(k);

    // Loop on package list
    // =========================================================================

    for i=1:size(packages_list,"*")

        // packages_list(i) is the technical name of the current package

        // Get the list of versions compatibles with this version of Scilab
        this_package_versions = atomsCompatibleVersions(packages_list(i));

        // If no compatible version, no need to add this package to the list
        if isempty(this_package_versions) then
            continue;
        end

        // Add the package to the list
        if nameonly then
            packages = [ packages;packages_list(i)];
        else
            packages = [ packages; ..
            emptystr(size(this_package_versions,"*"),1)+packages_list(i), ..
            this_package_versions ];
        end
    end

endfunction
