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

// End user function

function res = atomsGetLoadedPath(packages)

    rhs           = argn(2);
    res           = [];

    // Check number of input arguments
    // =========================================================================

    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsGetLoadedPath",1));
    end

    // Check input parameters type
    // =========================================================================

    if type(packages) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsGetLoadedPath",1));
    end

    if size(packages(1,:),"*") > 3 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: mx1,mx2 or mx3 string matrix expected.\n"),"atomsGetLoadedPath",1));
    end

    packages = stripblanks(packages);

    // Complete packages matrix with empty columns to have a mx3 matrix
    // =========================================================================

    if size(packages(1,:),"*") == 1 then
        packages = [ packages emptystr(size(packages(:,1),"*"),1) emptystr(size(packages(:,1),"*"),1) ];

    elseif size(packages(1,:),"*") == 2 then
        packages = [ packages emptystr(size(packages(:,1),"*"),1) ];

    end

    // Get the list of installed packages
    // =========================================================================
    loadedpackages = atomsGetLoaded();

    // Loop on name
    // =========================================================================

    for i=1:size(packages(:,1),"*")

        if ~ atomsIsLoaded(packages(i,:)) then
            res(i) = "";

        else
            // Filter on names
            packages_filtered = loadedpackages( find(loadedpackages(:,1) == packages(i,1)),:);

            res(i) = packages_filtered(1,4);
        end
    end

endfunction
