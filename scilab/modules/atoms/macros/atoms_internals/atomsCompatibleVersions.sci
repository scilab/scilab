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

// Return the sorted list of the versions compatible with this version of Scilab

function versions = atomsCompatibleVersions(name)

    // Initialize
    versions = [];

    // Check input parameters
    // =========================================================================

    rhs  = argn(2);

    if rhs <> 1  then
        error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsCompatibleVersions",1));
    end

    if type(name) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"),"atomsCompatibleVersions",1));
    end

    // Get all package description
    // =========================================================================
    allpackages = atomsDESCRIPTIONget();

    // Check if it's a Atoms package
    // =========================================================================
    if ~ isfield(allpackages,name) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be an atoms package name.\n"),"atomsCompatibleVersions",1));
    end

    // Get versions
    // =========================================================================
    package_versions          = allpackages(name);
    package_versions_tab      = getfield(1,package_versions);
    package_versions_tab(1:2) = [];

    // Loop on package_versions_tab
    // =========================================================================

    for i=1:size(package_versions_tab,"*")
        this_version = package_versions(package_versions_tab(i));
        versions = [ versions ; package_versions_tab(i) ];
    end

    // Sort the version matrix
    // =========================================================================
    versions = atomsVersionSort(versions,"DESC");

endfunction
