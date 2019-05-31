// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2019 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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

    if and(size(packages, 2) <> [1 2 3]) then
        error(msprintf(gettext("%s: Argument #%d: Matrix with %s columns expected.\n"),"atomsGetInstalledPath",1, _("1 to 3")));
    end

    packages = stripblanks(packages);

    if size(packages,2)==1 then
        packages = [packages emptystr(packages)]
    end

    // Allusers/user management
    // =========================================================================
    if size(packages, 2) < 3 then
        if rhs < 2 then
            section = "all";
        else
            // Process the 2nd input argument : section
            // Allusers can be equal to "user" or "allusers"
            if type(section) <> 10 then
                error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsGetInstalledPath",2));
            end
            section = section(1)
            if and(section <> ["user","allusers","all"]) then
                error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'' or ''allusers'' or ''all'' expected.\n"),"atomsGetInstalledPath",2));
            end
        end
        packages = [packages emptystr(packages(:,1)) + section]
    else
        tmp = convstr(packages(:,3))
        if or(tmp<>"user" & tmp<>"allusers" & tmp<>"all")
            error(msprintf(gettext("%s: Argument #%d (:,3): ''user'' or ''allusers'' or ''all'' expected.\n"),"atomsGetInstalledPath",1));
        end
        packages(:,3) = tmp
        section = "all"
    end

    // Get the list of installed packages
    // =========================================================================
    installedpackages = atomsGetInstalled(section);
    names = convstr(installedpackages(:,1));    // for case-insensitive detection

    // Loop on name
    // =========================================================================

    for i = 1:size(packages,1)

        // Filter on names
        found = find(installedpackages(:,1) == packages(i,1))
        if found == []
            // Try in a case-insensitive way
            found = find(names == convstr(packages(i,1)))
        end
        if found == []
            // Try with input as a case-insensitive fragment
            found = grep(names, convstr(packages(i,1)))
        end
        if found == []
            res(i) = "";
            continue;
        end
        packages_filtered = installedpackages(found, : );

        // Filter on section
        if packages(i,3) <> "all" then
            packages_filtered = packages_filtered( find(packages_filtered(:,3) == packages(i,3)) , : );
        end

        // Filter on versions
        //  + The packaging version is mentioned
        if packages_filtered <> [] & packages(i,2) <> ""
            if ~ isempty(strindex(packages(i,2),"-")) then
                packages_filtered = packages_filtered( find(packages_filtered(:,2) == packages(i,2)) , : );
            else
                //  + The packaging version is not mentioned
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
        end

        if packages_filtered == [] then
            res(i) = "";
            continue;
        end

        if ~isempty(packages_filtered) then
            res(i) = packages_filtered(1,4);
        else
            res(i) = "";
        end
    end
endfunction
