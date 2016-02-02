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

// display of the available toolboxes

function atomsList(category)

    // Load Atoms Internals lib if it's not already loaded
    // =========================================================================
    if ~ exists("atomsinternalslib") then
        load("SCI/modules/atoms/macros/atoms_internals/lib");
    end

    // Check input parameters
    // =========================================================================

    rhs = argn(2);

    if rhs > 1 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d or %d expected.\n"),"atomsInstall",0,1))
    end


    if rhs==1 then

        if type(category) <> 10 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsIsCategory",1));
        end

        if size(category,"*") <> 1 then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: string expected.\n"),"atomsIsCategory",1));
        end

        if ~atomsIsCategory(category) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: A valid category expected.\n"),"atomsIsCategory",1));
        end

    end

    // Get the list of available toolboxes
    // =========================================================================

    [packages_struct,categories_struct] = atomsDESCRIPTIONget();

    if rhs==0 then
        packages_list      = getfield(1,packages_struct);
        packages_list(1:2) = [];
        packages_list      = packages_list';
    else
        category_struct    = categories_struct(category);
        packages_list      = category_struct("packages");
        packages_list      = unique(packages_list(:,1));
    end

    [A,k]         = gsort(convstr(packages_list,"l"),"lr","i");
    packages_list = packages_list(k);
    packages_disp = [];

    // Loop on package list
    // =========================================================================

    for i=1:size(packages_list,"*")

        this_package_name = packages_list(i);

        // Get the list of versions compatibles with this version of Scilab
        this_package_versions = atomsCompatibleVersions(packages_list(i));

        if isempty(this_package_versions) then
            continue;
        end

        // Use the Most Recent Version
        this_package_version = this_package_versions(1);

        // Get the details of this toolbox
        this_package_details = atomsToolboxDetails([this_package_name,this_package_version]);
        this_package_summary = this_package_details("Summary");

        packages_disp        = [ packages_disp ; this_package_name this_package_summary ];

    end

    // Loop on package list ... a second time
    // =========================================================================

    for i=1:size(packages_disp(:,1),"*")
        mprintf("% "+string(max(length(packages_disp(:,1))))+"s - %s\n",packages_disp(i,1),packages_disp(i,2));
    end

endfunction
