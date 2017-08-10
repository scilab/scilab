// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2010 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
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

// Remove a toolbox

function result = atomsRemove(packages,section,del)

    result = [];

    // Load Atoms Internals lib if it's not already loaded
    // =========================================================================
    if ~ exists("atomsinternalslib") then
        load("SCI/modules/atoms/macros/atoms_internals/lib");
    end

    // Check write access on allusers zone
    // =========================================================================
    ATOMSALLUSERSWRITEACCESS = atomsAUWriteAccess();

    // Save the initial path
    // =========================================================================
    ATOMSINITIALPATH = pwd();

    // Get scilab version (needed for later)
    // =========================================================================
    sciversion = strcat(string(getversion("scilab")) + ".");

    // Check input parameters
    // =========================================================================

    rhs = argn(2);

    if rhs < 1 | rhs > 3 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"atomsRemove",1,3))
    end

    if type(packages) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsRemove",1));
    end

    if size(packages(1,:),"*") > 2 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: mx1 or mx2 string matrix expected.\n"),"atomsInstall",1));
    end

    packages = stripblanks(packages);

    // If mx1 matrix, add a 2nd column with empty versions
    // =========================================================================
    if size(packages(1,:),"*") == 1 then
        packages = [ packages emptystr(size(packages(:,1),"*"),1) ];
    end

    // Operating system detection + Architecture detection
    // =========================================================================
    [OSNAME,ARCH,LINUX,MACOSX,SOLARIS,BSD] = atomsGetPlatform();

    // Verbose Mode ?
    // =========================================================================
    if strcmp(atomsGetConfig("Verbose"),"True", "i") == 0 then
        ATOMSVERBOSE = %T;
    else
        ATOMSVERBOSE = %F;
    end

    // Allusers/user management
    //   - If section is equal to "all" or to True, packages located in both
    //     "allusers" and "user" sections will removed.
    //   - If section is equal to "allusers", only packages located in the
    //     "allusers" section will be removed.
    //   - If section is equal to "user" or to False, only packages located in
    //     the "user" will be removed
    // =========================================================================

    if rhs <= 1 then

        // By default:
        //  → Remove packages located in both "allusers" and "user" sections if
        //    we have the write access to SCI directory
        //  → Remove only package located in the "user" sections otherwise

        if ATOMSALLUSERSWRITEACCESS then
            section = "all";
        else
            section = "user";
        end

        del = %F;

    elseif rhs==2 then

        // Process the 2nd input argument : section
        // Allusers can be a boolean or equal to "user" or "allusers"

        if typeof(section) <> "string" & typeof(section) <> "boolean" then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: A boolean or single string expected.\n"),"atomsRemove",2));
        end

        if size(section, "*") <> 1 then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: A boolean or single string expected.\n"),"atomsRemove",2));
        end

        if typeof(section) == "string" then
            if and(section<>["user","allusers","all"]) then
                error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'' or ''allusers'' or ''all'' expected.\n"),"atomsRemove",1));
            end

            // Check if we have the write access
            if or(section==["all","allusers"]) & ~ ATOMSALLUSERSWRITEACCESS then
                error(msprintf(gettext("%s: You haven''t write access on this directory : %s.\n"),"atomsRemove",2,atomsPath("system","user")));
            end

            del = %F;
        else
            del = section;
            if ATOMSALLUSERSWRITEACCESS then
                section = "all";
            else
                section = "user";
            end
        end

    else

        if typeof(section) <> "string" then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: A boolean or single string expected.\n"),"atomsRemove",2));
        end

        if size(section, "*") <> 1 then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: A boolean or single string expected.\n"),"atomsRemove",2));
        end

        // Process the 3rd input argument : delete
        if typeof(del) <> "boolean" then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: A boolean expected.\n"),"atomsRemove",3));
        end

        if size(del, "*") <> 1 then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: A boolean expected.\n"),"atomsRemove",3));
        end

        if typeof(section) == "string" then
            if and(section<>["user","allusers","all"]) then
                error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'' or ''allusers'' or ''all'' expected.\n"),"atomsRemove",1));
            end

            // Check if we have the write access
            if or(section==["all","allusers"]) & ~ ATOMSALLUSERSWRITEACCESS then
                error(msprintf(gettext("%s: You haven''t write access on this directory : %s.\n"),"atomsRemove",2,atomsPath("system","user")));
            end
        end
    end

    // Some checking on packages variable
    // =========================================================================

    for i=1:size(packages(:,1),"*")

        package_names(i)    = packages(i,1);
        package_versions(i) = packages(i,2);

        // Check if this package is installed
        if ~ atomsIsInstalled([package_names(i) package_versions(i)],section) then

            // Print a warning if the package isn't installed

            if isempty(package_versions(i)) then
                atomsDisp(msprintf("\t%s isn''t installed",package_names(i)));
            else
                atomsDisp(msprintf("\t%s (%s) isn''t installed",package_names(i),package_versions(i)));
            end

        elseif (section=="user") & (~ isempty(package_versions(i)) ) then

            // The package is installed, now check if we have the right to
            // uninstall it

            installed_details = atomsGetInstalledDetails(packages(i,:),section);

            if installed_details(3) == "allusers" then
                error(msprintf(gettext("%s: You have not enough rights to remove the package %s (%s).\n"),"atomsRemove",package_names(i),package_versions(i)));
            end

        elseif (section=="user") & isempty(package_versions(i)) then

            // Check if we have the right to remove at least one of the version
            // of the package
            if isempty(atomsGetInstalledVers(package_names(i),section)) then
                error(msprintf(gettext("%s: You have not enough rights to remove any version of the package %s.\n"),"atomsRemove",package_names(i)));
            end

        end

    end

    // Build the list of package to Uninstall
    // =========================================================================
    remove_package_list = atomsRemoveList(packages,section);
    // Loop on remList to print if a package has to be remove
    // or not
    // =========================================================================
    if ATOMSVERBOSE
        for i=1:size(remove_package_list(:,1),"*")
            if remove_package_list(i,1) == "-" then
                if del==%T then
                    atomsDisp(msprintf("\t%s (%s) will be removed from the ''%s'' section and its package fully deleted",remove_package_list(i,3),remove_package_list(i,4),remove_package_list(i,5)));
                else
                    atomsDisp(msprintf("\t%s (%s) will be removed from the ''%s'' section",remove_package_list(i,3),remove_package_list(i,4),remove_package_list(i,5)));
                end
            elseif (remove_package_list(i,1) == "~") & (remove_package_list(i,1) == "B") then
                atomsDisp(msprintf("\t%s (%s) cannot be removed from the ''%s'' section and will be broken",remove_package_list(i,3),remove_package_list(i,4),remove_package_list(i,5)));
            end
        end
    end

    // Now we have the list of package that have to be uninstalled
    // =========================================================================

    for i=1:size(remove_package_list(:,1),"*")

        // If the package must be keeped, the job is done
        if remove_package_list(i,1) <> "-" then
            continue;
        end

        this_package_name      = remove_package_list(i,3);
        this_package_version   = remove_package_list(i,4);
        this_package_section   = remove_package_list(i,5);

        // Add the package to list of package to remove
        atomsToremoveRegister(this_package_name,this_package_version,this_package_section);

        // Check if the package is loaded or not
        if atomsIsLoaded([this_package_name this_package_version]) then
            mprintf( "\tthe package %s (%s) is currently loaded, It will be removed at next Scilab start\n" , this_package_name , this_package_version );
            continue;
        end

        atomsDisp(msprintf(gettext("Removing %s (%s)(%s).\n\n"), this_package_name , this_package_version , this_package_section));

        // Online or offline, we only need the information registered in
        load(atomsPath("system",this_package_section)+"installed.bin");
        this_package_insdet    = installed_struct(this_package_name+" - "+this_package_version)';
        this_package_directory = this_package_insdet(3);

        // switching section and path columns to get the same output format as usual
        tmp = this_package_insdet(3);
        this_package_insdet(3) = this_package_insdet(4);
        this_package_insdet(4) = tmp;
        clear installed_struct;

        // Check if this_package_directory start with SCI or SCIHOME

        if  (grep(this_package_directory,pathconvert(SCI)) == []) & ..
            (grep(this_package_directory,pathconvert(SCIHOME)) == []) &..
            (grep(this_package_directory,"/^(SCI|SCIHOME)\"+filesep()+"/","r") == []) then

            atomsError("error", ..
            msprintf( ..
            gettext("%s: The directory of this package (%s-%s) is located neither in SCI nor in SCIHOME. For security reason, ATOMS refuses to delete this directory.\n"), ..
            "atomsRemove", ..
            this_package_name, ..
            this_package_version));
        end

        if isdir(this_package_directory) then

            uninstall_status = rmdir(this_package_directory,"s");

            if uninstall_status<>1 then
                atomsError("error", ..
                msprintf( ..
                gettext("%s: The directory of this package (%s-%s) cannot been deleted, please check if you have write access on this directory : %s.\n"), ..
                "atomsRemove", ..
                this_package_name, ..
                this_package_version, ..
                strsubst(this_package_directory,"\","\\") ));
            end

        end

        // Check if the parent directory (directory name == toolbox name ) is empty
        // If yes, delete it
        // =====================================================================
        this_package_root_dir = part(this_package_directory,1:length(pathconvert(this_package_directory,%t,%f)) - length(this_package_version) - 1 );

        if isdir(this_package_root_dir) & listfiles(this_package_root_dir)==[] then
            stat = rmdir(this_package_root_dir);
            if stat<>1 then
                atomsError("error", ..
                msprintf( ..
                gettext("%s: The root directory of this package (%s-%s) cannot been deleted, please check if you have write access on this directory : %s.\n"), ..
                "atomsRemove", ..
                this_package_name, ..
                this_package_version, ..
                strsubst(this_package_root_dir,"\","\\") ));
            end
        end

        // Remove the tarball file if asked
        if del==%T then
            atomsDisp(msprintf(gettext("Deleting archives files for %s.\n\n"),this_package_name));
            atomsDeleteTarball(this_package_name,section,this_package_version);
        end

        // Remove this toolbox from the list of installed packages
        // =====================================================================
        atomsInstallUnregister(this_package_name,this_package_version,this_package_section);

        // Remove this toolbox from the list of autoloaded packages
        // =====================================================================
        atomsAutoloadDel([this_package_name this_package_version this_package_section]);

        // Remove it from the DESCRIPTION_installed file
        // =====================================================================

        DESCRIPTION_file = atomsPath("system",this_package_section) + "DESCRIPTION_installed";

        if ~ isempty(fileinfo(DESCRIPTION_file)) then
            DESCRIPTION = atomsDESCRIPTIONread(DESCRIPTION_file);
            DESCRIPTION = atomsDESCRIPTIONrm(DESCRIPTION,this_package_name,this_package_version);
            atomsDESCRIPTIONwrite(DESCRIPTION,DESCRIPTION_file);
        end

        // Del the package from the list of package to remove
        // =====================================================================
        atomsToremoveUnregister(this_package_name,this_package_version,this_package_section);

        // Fill the result matrix
        // =====================================================================

        result = [ result ; this_package_insdet ];

        // Success message if needed
        // =====================================================================
        atomsDisp(msprintf(" success"));
    end

    // Go to the initial location
    chdir(ATOMSINITIALPATH);

endfunction
