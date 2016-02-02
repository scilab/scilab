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

// Output arguments :

//   insList  : . List  of the changes caused by the installation of one or
//                more packages
//              . matrix ( n x 4 )
//              . mandatory
//              . example :
//                 !~  U  toolbox_4  1.0  !
//                 !                      !
//                 !~     toolbox_2  1.3  !
//                 !                      !
//                 !~     toolbox_1  1.9  !
//                 !                      !
//                 !~  U  toolbox_5  1.0  !

//   tree_out : . Dependency tree of the package (returned by atomsDepTreeFlat)
//              . struct
//              . mandatory
//              . Example :
//                   tree_out  =
//                   toolbox_5 - 1.0: [1x1 struct]
//                   toolbox_4 - 1.0: [1x1 struct]
//                   toolbox_2 - 1.3: [1x1 struct]
//                   toolbox_1 - 1.9: [1x1 struct]

function [insList,depTree] = atomsInstallList(packages,section)

    insList = [];
    depTree = struct();

    // Get scilab version (needed for later)
    // =========================================================================
    sciversion = strcat(string(getversion("scilab")) + ".");

    // Check input parameters
    // =========================================================================

    rhs = argn(2);

    if rhs > 2 then
        error(msprintf(gettext("%s: Wrong number of input arguments: at most %d expected.\n"),"atomsInstallList",2))
    end

    // 1st input argument

    if type(packages) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsInstallList",1));
    end

    if size(packages(1,:),"*") > 2 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: mx1 or mx2 string matrix expected.\n"),"atomsInstall",1));
    end

    packages = stripblanks(packages);

    // 2nd input input argument, section management :
    //   - 1st case: section is not specified or is equal to "all", module
    //               is searched in both "user" and "allusers" section
    //   - 2nd case: section is equal to "user", module is only searched in
    //               the "user" section"
    //   - 3rd case: section is equal to "allusers", module is only searched
    //               in the "allusers" section"

    if rhs < 2 then
        section = "all";

    else

        if type(section) <> 10 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsInstallList",2));
        end

        if and(section<>["user","allusers","all"]) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'' or ''allusers'' expected.\n"),"atomsInstallList",2));
        end

    end

    // If mx1 matrix, add a 2nd column with empty versions
    // =========================================================================
    if size(packages(1,:),"*") == 1 then
        packages = [ packages emptystr(size(packages(:,1),"*"),1) ];
    end

    // Loop on packages and to build the dependency tree
    // =========================================================================

    for i=1:size(packages(:,1),"*")

        this_package_name    = packages(i,1);
        this_package_version = packages(i,2);

        // Now check if it's a valid package
        if ~ atomsIsPackage(packages(i,:)) then
            if isempty(this_package_version) then
                module_full_name = this_package_name;
            else
                module_full_name = this_package_name+" - "+this_package_version;
            end
            atomsError("error", ..
            msprintf(gettext("%s: The package %s is not available.\n"),"atomsInstallList",module_full_name));
        end

        // Fill the version if it doesn't contain the packaging version
        this_package_version = atomsPackagingVersion(packages(i,:));
        packages(i,2)        = this_package_version;

        // Build the depencency tree
        [tree,version_out]  = atomsDepTreeFlat(this_package_name,this_package_version);

        if (type(tree) == 4) & (~ tree) then
            atomsError("error", ..
            msprintf(gettext("%s: The dependency tree cannot be resolved.\n"),"atomsInstallList",1));
        end

        // Update the  package_versions(i) with the version returned by
        // atomsDepTreeFlat
        packages(i,2) = version_out;

        // Concatenate the tree with the existing one
        depTree = atomsCatTree( depTree , tree );
    end

    // Add a field to detect later if it's the toolbox is automatically installed
    // or if it's a user choice
    // =========================================================================

    for i=1:size(packages(:,1),"*")
        this_package_details                     = depTree(packages(i,1)+" - "+packages(i,2));
        this_package_details("user_choice")      = %T;
        depTree(packages(i,1)+" - "+packages(i,2)) = this_package_details;
    end

    // Now we have the list of package that have to be installed
    // We have to check if
    //  - each package is already installed
    //  - If yes, Is it the most recent version
    // =========================================================================

    mandatory_packages      = getfield(1,depTree);
    mandatory_packages(1:2) = [];

    for i=1:size(mandatory_packages,"*")

        this_package_details = depTree(mandatory_packages(i));

        this_package_name    = this_package_details("Toolbox");
        this_package_version = this_package_details("Version");

        if isfield(this_package_details,"user_choice") & this_package_details("user_choice") then
            this_package_user_choice = "U"; // stand for User Choice
        else
            this_package_user_choice = ""; // stand for User Choice
        end

        to_install = %F;

        // Now, it's time to check if the module is installed or not :
        //   - 1st case: section is not specified or is equal to "all", module
        //               is searched in both "user" and "allusers" section
        //   - 2nd case: section is equal to "user", module is only searched in
        //               the "user" section"
        //   - 3rd case: section is equal to "allusers", module is only searched
        //               in the "allusers" section"

        if atomsIsInstalled(this_package_name,section) then
            vers = atomsGetInstalledVers(this_package_name,section);
            if find( vers == this_package_version ) == [] then
                to_install = %T;
            end
        else
            to_install = %T;
        end

        if to_install then
            insList = [ insList ; "+" this_package_user_choice this_package_name this_package_version ];
        else
            insList = [ insList ; "~" this_package_user_choice this_package_name this_package_version ];
        end

    end

endfunction

// =============================================================================
// Return the version filled with the packaging version
// → The result is based on the packages struct returned by atomsDESCRIPTIONget(),
//   not on the installed packages.
// → If a package doesn't exist in the packages struct, an empty string is
//   returned
// =============================================================================

function result = atomsPackagingVersion(packages)

    // Initialize
    // =========================================================================
    result = [];

    // Check input parameters
    // =========================================================================
    rhs  = argn(2);

    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsPackagingVersion",1));
    end

    if type(packages) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsPackagingVersion",1));
    end

    if size(packages(1,:),"*") <> 2 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: mx2 string matrix expected.\n"),"atomsPackagingVersion",1));
    end

    // Get all package description
    // =========================================================================
    allpackages = atomsDESCRIPTIONget();

    // Loop on packages
    // =========================================================================

    for i=1:size(packages(:,1),"*")

        result(i) = "",
        name      = packages(i,1);
        version   = packages(i,2);

        // Check if the name exists in the packages struct
        if ~ isfield(allpackages,name) then
            result(i) = "";
            break;
        end

        // 1st case : no version is mentioned
        if isempty(version) then
            result(i) = "";

            // 2nd case : the packaging version is already mentioned
        elseif ~ isempty(strindex(version,"-")) then
            result(i) = version;

            // 3rd case : the packaging version is not mentioned
        else

            // Loop on this packages versions
            package_versions          = allpackages(name);
            package_versions_tab      = getfield(1,package_versions);
            package_versions_tab(1:2) = [];

            for j=1:size(package_versions_tab,"*")

                if isempty( strindex(package_versions_tab(j),"-") ) then
                    if package_versions_tab(j) == version then
                        result(i) = version;
                        break;
                    end

                else
                    if part(package_versions_tab(j), 1:strindex(package_versions_tab(j),"-")-1)==version then
                        result(i) = package_versions_tab(j);
                        break;
                    end

                end

            end
        end

    end

endfunction
