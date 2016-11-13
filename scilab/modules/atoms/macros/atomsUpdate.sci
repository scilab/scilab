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

// update of a toolbox

function result = atomsUpdate(name,section)

    // Load Atoms Internals lib if it's not already loaded
    // =========================================================================
    if ~ exists("atomsinternalslib") then
        load("SCI/modules/atoms/macros/atoms_internals/lib");
    end

    result = [];

    // Verbose Mode ?
    // =========================================================================
    if strcmp(atomsGetConfig("Verbose"),"True", "i") == 0 then
        ATOMSVERBOSE = %T;
    else
        ATOMSVERBOSE = %F;
    end

    // Check write access on allusers zone
    // =========================================================================
    ATOMSALLUSERSWRITEACCESS = atomsAUWriteAccess();

    // Check input parameters
    // =========================================================================

    rhs = argn(2);

    if rhs > 2 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"atomsUpdate",0,2))
    end

    if (rhs>0) & (~ isempty(name) ) & (type(name)<>10) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsUpdate",1));
    end

    // Install for all users or just for me ?
    // =========================================================================

    if rhs < 2 then
        // By default, install for all users (if we have write access of course !)
        if ATOMSALLUSERSWRITEACCESS then
            section = "all";
        else
            section = "user";
        end

    else
        // Just check if it's a boolean
        if type(section) <> 10 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsInstall",2));
        end

        // Check if we have the write access
        if (section=="allusers") & ~ ATOMSALLUSERSWRITEACCESS then
            error(msprintf(gettext("%s: You haven''t write access on this directory : %s.\n"),"atomsInstall",2,atomsPath("system","allusers")));
        end
    end

    // Check if all specified toolboxes are effectively installed
    // =========================================================================

    if (rhs>0) &  ~isempty(name) then

        // Remove leading and trealing whitespaces
        name = stripblanks(name);

        for i=1:size(name,"*")
            if ~ atomsIsInstalled(name(i)) then
                error(msprintf(gettext("%s: ''%s'' isn''t installed.\n"),"atomsUpdate",name(i)));
            end
        end
    end

    // Get scilab version (needed for later)
    // =========================================================================
    sciversion = strcat(string(getversion("scilab")) + ".");

    // If name isn't defined or empty, get the full list of installed packages
    // =========================================================================

    if (rhs==0) | isempty(name) then

        name               = [];
        package_installed  = atomsGetInstalled(section);
        package_installed  = package_installed( find( package_installed(:,5) == "I") , : );

        for i=1:size(package_installed(:,1),"*")
            if find( name == package_installed(i,1) ) == [] then
                name = [ name ;  package_installed(i,1) ];
            end
        end
    end

    // Build the list of explicit packages to update (i.e without dependencies)
    // =========================================================================

    package_main_list = [];

    for i=1:size(name,"*")

        if section=="all" then
            sections = ["user";"allusers"];
        else
            sections = section;
        end

        for j=1:size(sections,"*")

            if ~atomsIsInstalled(name(i),sections(j)) then
                continue;
            end

            this_package_versions    = atomsGetInstalledVers(name(i),sections(j));
            package_main_list        = [ package_main_list ; atomsGetInstalledDetails([name(i) this_package_versions(1)],sections(j)) ];

        end

    end

    // Install the Most Recent Version of all items of "name" array
    // =========================================================================

    for i=1:size(package_main_list(:,1),"*")

        this_package_MRV_ins     = package_main_list(i,2);     // Most Recent Version Installed
        this_package_MRV_ava     = atomsGetMRVersion(name(i)); // Most Recent Version Available
        this_package_ins_section = package_main_list(i,3);

        if (this_package_MRV_ava == -1) | ..
            ( atomsVersionCompare(this_package_MRV_ins,this_package_MRV_ava) == 0 ) then
            // The installed version is already the Most Recent Version Available
            atomsDisp(msprintf("\t%s (%s) : The most recent version is already installed",name(i),this_package_MRV_ins));
        else
            // Install the new toolbox
            this_result = atomsInstall([name(i) this_package_MRV_ava],this_package_ins_section);

            // Fill the output argument
            result = [ result ; this_result ];
        end

    end

    // Loop on name to update dependencies
    // =========================================================================

    for i=1:size(package_main_list(:,1),"*")

        this_package_MRV_ins     = package_main_list(i,2);     // Most Recent Version Installed
        this_package_MRV_ava     = atomsGetMRVersion(name(i)); // Most Recent Version Available
        this_package_ins_section = package_main_list(i,3);

        // Now check if it's dependencies are up-to-date
        dependencies = atomsInstallList([name(i) this_package_MRV_ins],this_package_ins_section);

        for j=1:size(dependencies(:,1),"*")

            if ~atomsIsInstalled([dependencies(j,3) dependencies(j,4)],this_package_ins_section) then

                // Install the new toolbox
                this_result = atomsInstall([dependencies(j,3) dependencies(j,4)],this_package_ins_section);

                // It's just a dependency
                atomsInstallUnregister(dependencies(j,3),dependencies(j,4),this_package_ins_section);
                atomsInstallRegister(dependencies(j,3),dependencies(j,4),"A",this_package_ins_section);

                // Fill the output argument
                result = [ result ; this_result ];
            end

        end

    end

    // Remove older version of all items of "name" array
    // =========================================================================

    for i=1:size(package_main_list(:,1),"*")

        this_package_name        = package_main_list(i,1);
        this_package_ins_section = package_main_list(i,3);
        this_package_versions    = atomsGetInstalledVers(this_package_name,this_package_ins_section);
        this_package_versions(1) = []; // Don't remove the most recent version

        for j=1:size(this_package_versions,"*")

            this_package_version = this_package_versions(j);

            // Check if this package is a child of another installed package
            if isempty( atomsGetDepParents([this_package_name this_package_version],this_package_ins_section) ) then
                atomsRemove([this_package_name this_package_version],this_package_ins_section);
            end

        end

    end

endfunction
