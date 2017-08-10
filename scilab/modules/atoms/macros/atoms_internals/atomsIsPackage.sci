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

// Return true if the package is a valid package or not

function result = atomsIsPackage(packages)

    // Initialize
    // =========================================================================
    result = [];

    // Check input parameters
    // =========================================================================
    rhs  = argn(2);

    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsIsPackage",1));
    end

    if type(packages) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsIsPackage",1));
    end

    if size(packages(1,:),"*") > 2 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: mx1 or mx2 string matrix expected.\n"),"atomsIsPackage",1));
    end

    // If packages is mx1 matrix, add a 2nd column with empty versions
    // =========================================================================
    if size(packages(1,:),"*") == 1 then
        packages = [ packages emptystr(size(packages(:,1),"*"),1) ];
    end

    // Get all package description
    // =========================================================================
    allpackages = atomsDESCRIPTIONget();

    // Loop on packages
    // =========================================================================

    for i=1:size(packages(:,1),"*")

        result(i) = %F,
        name      = packages(i,1);
        version   = packages(i,2);

        // 1st case : just test the name, not the version
        if isempty(version) & isfield(allpackages,name) then
            result(i) = %T;

            // 2nd case : Check a specific version
        elseif isfield(allpackages,name) then

            // A packaging version is mentionned
            if strindex(version,"-")<>[] then
                result(i) = isfield(allpackages(name),version);

            else

                // Maybe the version without packaging version exist ...
                if isfield(allpackages(name),version) then
                    result(i) = %T;

                    // ... second try if it's not the case
                else
                    // Loop on this packages versions
                    package_versions          = allpackages(name);
                    package_versions_tab      = getfield(1,package_versions);
                    package_versions_tab(1:2) = [];

                    for j=1:size(package_versions_tab,"*")

                        // Split version and packaging version
                        version_mat  = strsubst(strsplit(package_versions_tab(j),strindex(package_versions_tab(j),"-")) , "-" , "" );

                        if version == version_mat(1) then
                            result(i) = %T;
                            break;
                        end

                    end
                end

            end
        end

    end

endfunction
