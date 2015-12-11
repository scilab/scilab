// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
