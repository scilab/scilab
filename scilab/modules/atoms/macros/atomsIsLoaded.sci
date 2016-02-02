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

// Output arguments :

//   res  :  .%T if the toolbox is loaded in this scilab session
//           . Matrix of boolean ( n x 1 )
//           . mandatory

//   version_out : . If the toolbox is loaded, version_out is the version of the
//                   loaded toolbox
//                   otherwise, version_out = ""
//                 . Matrix of string (n x 1)
//                 . mandatory

function [res,version_out] = atomsIsLoaded(packages)

    // Load Atoms Internals lib if it's not already loaded
    // =========================================================================
    if ~ exists("atomsinternalslib") then
        load("SCI/modules/atoms/macros/atoms_internals/lib");
    end

    rhs         = argn(2);
    lhs         = argn(1);
    res         = [];
    version_out = [];

    // Check number of input arguments
    // =========================================================================

    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsIsLoaded",1));
    end

    // Check input parameters type
    // =========================================================================

    if type(packages) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsIsLoaded",1));
    end

    if size(packages(1,:),"*") > 3 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: mx1,mx2 or mx3 string matrix expected.\n"),"atomsIsLoaded",1));
    end

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

        name    = packages(i,1);
        version = packages(i,2);
        section = packages(i,3);
        res(i)  = %F;

        if packages(i,3) == "all" then
            section = "";
        else
            section = packages(i,3);
        end

        if isempty(version) & isempty(section) then
            // Just check the name
            res(i) = or(loadedpackages(:,1) == name);

        elseif isempty(version) & ~isempty(section) then
            // Filter on names
            packages_filtered = loadedpackages( find(loadedpackages(:,1) == name) , 3 );

            // Check if the wanted section is present
            res(i) = or(packages_filtered == section);

        elseif ~isempty(version) & isempty(section) then
            // Filter on names
            packages_filtered = loadedpackages( find(loadedpackages(:,1) == name) , 2 );

            // Check if the wanted version is present
            res(i) = or(packages_filtered == version);

            // Filter on versions

            //  + The packaging version is mentioned
            if ~ isempty(strindex(version,"-")) then
                res(i) = or(packages_filtered == version);

                //  + The packaging version is not mentioned
            else
                // Loop on installed versions
                for j=1:size(packages_filtered,"*")

                    if ~ isempty(strindex(packages_filtered(j),"-")) then
                        packages_filtered(j) = part(packages_filtered(j),1:strindex(packages_filtered(j),"-")-1);
                    end

                    if version == packages_filtered(j) then
                        res(i) = %T;
                        break;
                    end

                end

            end

        else
            // Filter on names
            packages_filtered = loadedpackages( find(loadedpackages(:,1) == name) , [2 3] );

            // Filter on section
            packages_filtered = packages_filtered( find(packages_filtered(:,2) == section) , 1 );

            // Filter on versions

            //  + The packaging version is mentioned
            if ~ isempty(strindex(version,"-")) then
                res(i) = or(packages_filtered == version);

                //  + The packaging version is not mentioned
            else
                // Loop on installed versions
                for j=1:size(packages_filtered,"*")

                    if ~ isempty(strindex(packages_filtered(j),"-")) then
                        packages_filtered(j) = part(packages_filtered(j),1:strindex(packages_filtered(j),"-")-1);
                    end

                    if version == packages_filtered(j) then
                        res(i) = %T;
                        break;
                    end

                end

            end

        end

        if lhs>1 then
            if res(i) then
                version_out(i) = loadedpackages( find(loadedpackages(:,1) == name) , 2 )
            else
                version_out(i) = "";
            end
        end

    end

endfunction
