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

// Return the most recent version of each package specified as first input argument

function versions = atomsGetMRVersion(packages)

    // Initialize
    versions = [];

    // Check input parameters
    // =========================================================================

    rhs  = argn(2);

    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument: at most %d expected.\n"),"atomsGetMRVersion",1));
    end

    if type(packages) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsGetMRVersion",1));
    end

    // Get all package description
    // =========================================================================

    allpackages = atomsDESCRIPTIONget();

    // Loop on packages
    // =========================================================================

    for i=1:size(packages,"*")

        versions(i) = "-1";

        if ~ isfield(allpackages,packages(i)) then
            continue;
        end

        package_versions          = allpackages(packages(i));
        package_versions_tab      = getfield(1,package_versions);
        package_versions_tab(1:2) = [];
        package_versions_tab      = atomsVersionSort( package_versions_tab , "DESC" );
        versions(i)               = package_versions_tab(1);

    end

endfunction
