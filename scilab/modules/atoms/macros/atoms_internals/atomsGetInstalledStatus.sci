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

// Liste des versions installée de la toolbox "name"

function res = atomsGetInstalledStatus(packages,section)

    rhs = argn(2);
    res = [];

    // Check number of input arguments
    // =========================================================================

    if rhs < 1 | rhs > 2 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsGetInstalledStatus",1,2));
    end

    // Check input parameters type
    // =========================================================================

    if type(packages) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsGetInstalledStatus",1));
    end

    if size(packages(1,:),"*") <> 2 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: mx2 string matrix expected.\n"),"atomsGetInstalledStatus",1));
    end

    // Allusers/user management
    // =========================================================================

    if rhs < 3 then
        section = "all";

    else

        // Process the 2nd input argument : section
        // Allusers can be a boolean or equal to "user" or "allusers"

        if type(section) <> 10 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Boolean or single string expected.\n"),"atomsGetInstalledStatus",2));
        end

        if and(section<>["user","allusers","all"]) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'' or ''allusers'' or ''all'' expected.\n"),"atomsGetInstalledStatus",2));
        end

    end

    // Get the list of installed packages
    // =========================================================================
    installedpackages = atomsGetInstalled(section);

    // Loop on name
    // =========================================================================

    for i=1:size(packages(:,1),"*")

        // Filter on names
        packages_filtered = installedpackages( find(installedpackages(:,1) == packages(i,1)) , : );

        // Filter on versions
        packages_filtered = packages_filtered( find(packages_filtered(:,2) == packages(i,2)) , : );

        if ~ isempty(packages_filtered) then
            res(i) = packages_filtered(1,5);
        else
            res(i) = "";
        end

    end

endfunction
