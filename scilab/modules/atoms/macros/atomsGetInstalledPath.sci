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

// Add an URL to the list of repositories, and returns

function res = atomsGetInstalledPath(packages,section)

    rhs           = argn(2);
    res           = [];
    installed     = []; // Column vector that contains user repositories

    // Check number of input arguments
    // =========================================================================

    if rhs < 1 | rhs > 2 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsGetInstalledPath",1,2));
    end

    // Check input parameters type
    // =========================================================================

    if type(packages) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsGetInstalledPath",1));
    end

    if and(size(packages(1,:),"*") <> [2 3]) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: mx2 or mx3 string matrix expected.\n"),"atomsGetInstalledPath",1));
    end

    packages = stripblanks(packages);

    if or(packages(:,2)=="")  then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: All modules version should be set.\n"),"atomsGetInstalledPath",1));
    end

    packages = stripblanks(packages);

    // Allusers/user management
    // =========================================================================

    if rhs < 2 then
        section = "all";

    else

        // Process the 2nd input argument : section
        // Allusers can be equal to "user" or "allusers"

        if type(section) <> 10 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Boolean or single string expected.\n"),"atomsGetInstalledPath",2));
        end

        if (type(section) == 10) & and(section<>["user","allusers","all"]) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'' or ''allusers'' or ''all'' expected.\n"),"atomsGetInstalledPath",2));
        end

    end

    // Complete packages matrix with empty columns
    // =========================================================================

    if size(packages(1,:),"*") == 2 then
        packages = [ packages emptystr(size(packages(:,1),"*"),1) ];
    end

    // Get the list of installed packages
    // =========================================================================
    installedpackages = atomsGetInstalled(section);

    // Loop on name
    // =========================================================================

    for i=1:size(packages(:,1),"*")

        // Filter on names
        packages_filtered = installedpackages( find(installedpackages(:,1) == packages(i,1)) , : );

        if packages_filtered == [] then
            res(i) = "";
            continue;
        end

        // Filter on section
        if ~isempty(packages(i,3)) & packages(i,3)<>"all" then
            packages_filtered = packages_filtered( find(packages_filtered(:,3) == packages(i,3)) , : );
        end

        // Filter on versions

        //  + The packaging version is mentioned
        if ~ isempty(strindex(packages(i,2),"-")) then
            packages_filtered = packages_filtered( find(packages_filtered(:,2) == packages(i,2)) , : );

            //  + The packaging version is not mentioned
        else

            candidates        = packages_filtered;
            packages_filtered = [];

            // Loop on installed versions
            for j=1:size(candidates(:,2),"*")

                candidate_version = candidates(j,2);

                if ~ isempty(strindex(candidate_version,"-")) then
                    candidate_version = part(candidate_version,1:strindex(candidate_version,"-")-1);
                end

                if packages(i,2) == candidate_version then
                    packages_filtered = [ packages_filtered ; candidates(j,:) ];
                end

            end

        end

        if packages_filtered == [] then
            res(i) = "";
            continue;
        end

        if ~ isempty(packages_filtered) then
            res(i) = packages_filtered(1,4);
        else
            res(i) = "";
        end

    end

endfunction
