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

// Installation of a toolbox

function result = atomsInstall(packages,section)

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

    if rhs < 1 | rhs > 2 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"atomsInstall",1,2))
    end

    if type(packages) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsInstall",1));
    end

    if size(packages(1,:),"*") > 2 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: mx1 or mx2 string matrix expected.\n"),"atomsInstall",1));
    end

    // Remove leading and trailing whitespace
    // =========================================================================
    packages = stripblanks(packages);

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
    //   - If Allusers is equal to "allusers", packages will installed in the "allusers" section :
    //       → SCI/contrib    : location of the packages
    //       → SCI/.atoms     : ATOMS system files
    //   - Otherwise, packages will installed in the "user" section :
    //       → SCIHOME/atoms  : location of the packages
    //       → SCIHOME/.atoms : location of the packages & ATOMS system files
    // =========================================================================

    if rhs <= 1 then

        // By default:
        //  → Install in the "allusers" section if we have the write access to
        //    SCI directory
        //  → Install in the "user" otherwise

        if ATOMSALLUSERSWRITEACCESS then
            section = "allusers";
        else
            section = "user";
        end

    else

        // Process the 2nd input argument : section
        // Allusers can be a boolean or equal to "user" or "allusers"

        if type(section) <> 10 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsInstall",2));
        end

        if and(section<>["user","allusers"]) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'' or ''allusers'' expected.\n"),"atomsInstall",2));
        end

        // Check if we have the write access
        if (section=="allusers") & ~ ATOMSALLUSERSWRITEACCESS then
            error(msprintf(gettext("%s: You haven''t write access on this directory : %s.\n"),"atomsInstall",pathconvert(SCI+"/.atoms")));
        end
    end

    // Create needed directories
    // =========================================================================
    atoms_system_directory  = atomsPath("system" ,section);
    atoms_install_directory = atomsPath("install",section);
    atoms_session_directory = atomsPath("system","session");
    atoms_tmp_directory     = pathconvert( atomsPath("system" ,section) + "tmp_" + sprintf("%d\n",getdate("s")) );

    directories2create = [  atoms_system_directory ;   ..
    atoms_install_directory ;  ..
    atoms_session_directory ;  ..
    atoms_tmp_directory ];

    for i=1:size(directories2create,"*")
        if ~ isdir( directories2create(i) ) & (mkdir( directories2create(i) ) <> 1) then
            error(msprintf( ..
            gettext("%s: The directory ''%s'' cannot been created, please check if you have write access on this directory.\n"),..
            "atomsInstall", directories2create(i)));
        end
    end

    // Define the "archives" directory path
    // Create it if it's not exist
    // =========================================================================
    archives_directory = atoms_install_directory + "archives";

    if ~ isdir( archives_directory ) & (mkdir( archives_directory ) <> 1) then
        error(msprintf( ..
        gettext("%s: The directory ''%s'' cannot been created, please check if you have write access on this directory.\n"),..
        "atomsInstall", ..
        archives_directory));
    end

    // Complete packages matrix with empty columns
    // =========================================================================

    if size(packages(1,:),"*") == 1 then
        packages = [ packages emptystr(size(packages(:,1),"*"),1) ];
    end

    // "Archive" installation
    // =========================================================================

    from_localarchive = %F;

    for i=1:size(packages(:,1),"*")
        this_package = packages(i,1);

        if ~ isempty(regexp(this_package,"/(\.tar\.gz|\.tgz|\.zip)$/","o")) then

            if fileinfo( this_package ) then
                error(msprintf(gettext("%s: The file ''%s'' does not exist or is not read accessible.\n"),"atomsInstall",this_package));
            end

            // expand filename - bug 10707
            this_package = pathconvert(this_package, %f);

            tmp_dir = atomsExtract(this_package,atoms_tmp_directory);
            tmp_dir = pathconvert(atoms_tmp_directory+tmp_dir);

            if fileinfo( tmp_dir + "DESCRIPTION" ) then
                error(msprintf(gettext("%s: DESCRIPTION file cannot be found in the package ''%s''\n"),"atomsInstall",this_package));
            end

            this_package_description = atomsDESCRIPTIONread(tmp_dir + "DESCRIPTION");
            this_package_packages    = this_package_description("packages");

            // Get package name and version
            // -----------------------------------------------------------------

            this_package_name    = getfield(1,this_package_packages);
            this_package_name    = this_package_name(3);

            this_package_version = getfield(1,this_package_packages(this_package_name));
            this_package_version = this_package_version(3);

            // Save the extracted directory
            // -----------------------------------------------------------------

            this_package_description = atomsDESCRIPTIONaddField( ..
            this_package_description, ..
            this_package_name,        ..
            this_package_version,     ..
            "extractedDirectory",     ..
            tmp_dir);

            this_package_description = atomsDESCRIPTIONaddField( ..
            this_package_description, ..
            this_package_name,        ..
            this_package_version,     ..
            "archiveFile",            ..
            this_package);

            this_package_description = atomsDESCRIPTIONaddField( ..
            this_package_description, ..
            this_package_name,        ..
            this_package_version,     ..
            "fromRepository",         ..
            "0");

            // Save the DESCRIPTION_archives
            // -----------------------------------------------------------------

            if fileinfo( atoms_tmp_directory + "DESCRIPTION_archives" )<>[] then
                packages_description = atomsDESCRIPTIONread(atomsPath("system","session")+"DESCRIPTION_archives");
                packages_description = atomsDESCRIPTIONcat(packages_description,this_package_description);
            else
                packages_description = this_package_description;
            end

            atomsDESCRIPTIONwrite(packages_description,atomsPath("system","session")+"DESCRIPTION_archives");

            // change the packages var
            // -----------------------------------------------------------------
            packages(i,:) = [ this_package_name this_package_version ];


            // Installation from a local archive
            // -----------------------------------------------------------------
            from_localarchive = %T;

        end

    end

    // Force update the system information
    // =========================================================================
    if from_localarchive then
        if (atomsGetConfig("offLine") == "True" | atomsGetConfig("offline") == "True") then
            warning(msprintf(gettext("Option offline of ATOMS configuration is set to True. atomsSystemUpdate did not check the latest modules availables.")));
        else
            atomsDESCRIPTIONget(%T);
        end
    end

    // Get the install list
    // =========================================================================
    [install_package_list,dependency_tree] = atomsInstallList(packages,section);

    // Loop on install_package_list to print if a package has to be installed
    // or not
    // =========================================================================
    for i=1:size(install_package_list(:,1),"*")
        if install_package_list(i,1) == "+" then
            atomsDisp(msprintf("\t%s (%s) will be installed in the ''%s'' section\n",install_package_list(i,3),install_package_list(i,4),section));
        elseif install_package_list(i,1) == "~" then
            atomsDisp(msprintf("\t%s (%s) is already installed in the ''%s'' section and up-to-date\n",install_package_list(i,3),install_package_list(i,4),section));
        end
    end

    // Now really install the packages
    // =========================================================================

    for i=1:size(install_package_list(:,1),"*")

        toarchive = %T;

        this_package_name    = install_package_list(i,3);
        this_package_version = install_package_list(i,4);

        this_package_details = dependency_tree(this_package_name+" - "+this_package_version);

        if install_package_list(i,1) <> "+" then
            continue;
        end

        atomsDisp(msprintf("\tInstalling %s (%s) ...",this_package_name,this_package_version));

        // Define the path of the directory where will be installed this toolbox
        // =====================================================================
        this_package_directory = atomsPath("install",section) + this_package_name + filesep();

        // Create the parent directory of this toolbox if it's not already exist
        // =====================================================================

        if ~isdir(this_package_directory) & (mkdir(this_package_directory)<>1) then
            atomsError("error", ..
            msprintf( gettext("%s: The directory ""%s"" cannot been created, please check if you have write access on this directory.\n"), ..
            "atomsInstall", ..
            strsubst(this_package_directory,"\","\\") ));
        end

        // "Repository" installation ; Download and Extract
        // =====================================================================

        if this_package_details("fromRepository") == "1" then

            // Define the path of the downloaded file
            // =================================================================

            if isfield(this_package_details,"binaryName") then
                fileprefix = "binary";
            elseif isfield(this_package_details,OSNAME+ARCH+"Name") then
                fileprefix = OSNAME+ARCH;
            else
                fileprefix = OSNAME;
            end

            fileout     = pathconvert(this_package_directory+this_package_details(fileprefix+"Name"),%F);
            filein      = this_package_details(fileprefix+"Url");
            filemd5     = this_package_details(fileprefix+"Md5");
            filearchive = archives_directory + filesep() + this_package_details(fileprefix+"Name");

            if isfile(filearchive) & getmd5(filearchive)==filemd5 then
                // Check if the file has already been successfully downloaded
                // =============================================================
                if copyfile( filearchive , this_package_directory ) <> 1 then
                    atomsError("error", ..
                    msprintf(gettext("%s: Error while copying the file ''%s'' to the directory ''%s''.\n"), ..
                    "atomsInstall", ..
                    strsubst(filearchive,"\","\\"), ..
                    strsubst(this_package_directory,"\","\\") ));
                end
                toarchive = %F;
            else
                // Launch the download
                // =============================================================
                atomsDownload(filein,fileout,filemd5);
            end

            // unarchive it
            // =================================================================
            this_package_details("extractedDirectory") = this_package_directory + atomsExtract(fileout,this_package_directory);
        end

        // Rename the created directory
        // =====================================================================

        if getos() == "Windows" then
            rename_cmd = "rename """+this_package_details("extractedDirectory")+""" """+this_package_version+"""";
        else
            rename_cmd = "mv """+this_package_details("extractedDirectory")+""" """+this_package_directory+this_package_version+"""";
        end

        [rep,stat,err]=unix_g(rename_cmd);

        if stat <> 0 then

            // Second try after a sleep
            // This is needed on windows platforms

            if getos() == "Windows" then
                sleep(2000);
                [rep,stat,err]=unix_g(rename_cmd);
            end

            if stat <> 0 then
                atomsError("error", ..
                msprintf(gettext("%s: Error while creating the directory ''%s''.\n"),..
                "atomsInstall", ..
                strsubst(pathconvert(this_package_directory+this_package_version),"\","\\") ));
            end

        end

        // Move the created directory
        // → Only under windows
        // → Only if it's a local package
        // =====================================================================

        if getos() == "Windows" & (this_package_details("fromRepository") == "0") then

            move_cmd = "move """+atoms_tmp_directory+this_package_version+""" """+pathconvert(this_package_directory,%F)+"""";

            [rep,stat,err]=unix_g(move_cmd);

            if stat <> 0 then

                // Second try after a sleep
                // This is needed on windows platforms

                if getos() == "Windows" then
                    sleep(2000);
                    [rep,stat,err]=unix_g(move_cmd);
                end

                atomsError("error", ..
                msprintf(gettext("%s: Error while creating the directory ''%s''.\n"),..
                "atomsInstall",..
                strsubst(pathconvert(this_package_directory+this_package_version),"\","\\") ));
            end

        end

        // Register the successfully installed package
        // =====================================================================

        if install_package_list(i,2) == "U" then
            // Intentionnaly Installed
            this_package_status = "I";
        else
            // Automatically installed
            this_package_status = "A";
        end

        atomsInstallRegister(this_package_name,this_package_version,this_package_status,section);

        // Autoload the toolbox unless precised
        // =====================================================================

        if ~ (atomsGetConfig("autoloadAddAfterInstall") == "False") then

            // Add a package to the autoload list only if it's intentionnaly
            // installed

            if this_package_status=="I" then
                // If another version is on the autoload list, remove it
                atomsAutoloadDel([this_package_name ""                   section],section);
                atomsAutoloadAdd([this_package_name this_package_version section],section);
            end

        end

        // Move the archive file (.tar.gz or .zip file) to the archive directory
        // =====================================================================

        if toarchive then
            if this_package_details("fromRepository")=="1" then
                this_package_archive = fileout;
            else
                this_package_archive = this_package_details("archiveFile");
            end

            if copyfile( this_package_archive , archives_directory ) <> 1 then
                if isfile(this_package_archive) then
                    atomsError("warning", ..
                    msprintf(gettext("%s: Previously existing file with the same name in ''%s''.\n"), ..
                    "atomsInstall", ..
                    strsubst(archives_directory,"\","\\") ));
                else
                    atomsError("error", ..
                    msprintf(gettext("%s: Error while copying the file ''%s'' to the directory ''%s''.\n"), ..
                    "atomsInstall", ..
                    strsubst(this_package_archive,"\","\\"), ..
                    strsubst(archives_directory,"\","\\") ));
                end
            end
        end

        if this_package_details("fromRepository")=="1" then
            mdelete( fileout );
        end

        // Fill the result matrix
        // =====================================================================
        result = [ result ; atomsGetInstalledDetails([this_package_name this_package_version]) ];

        // Save the description
        // Needed to remove the toolbox if it's has been removed from the
        // repository list
        // =====================================================================

        DESCRIPTION_file = atoms_system_directory+"DESCRIPTION_installed";

        if isempty(fileinfo(DESCRIPTION_file)) then
            DESCRIPTION = struct();
        else
            DESCRIPTION = atomsDESCRIPTIONread(DESCRIPTION_file);
        end

        DESCRIPTION = atomsDESCRIPTIONadd(DESCRIPTION,this_package_name,this_package_version,this_package_details);
        atomsDESCRIPTIONwrite(DESCRIPTION,DESCRIPTION_file);

        // Success message if needed
        // =====================================================================
        atomsDisp(msprintf(" success"));

    end

    // The TMPDIR DESCRIPTION_archives is no more needed
    // =========================================================================

    if ~ isempty(fileinfo(atoms_tmp_directory + "DESCRIPTION_archives")) then
        mdelete(atoms_tmp_directory + "DESCRIPTION_archives");
    end

    // The atoms_tmp_directory is no more needed
    // =========================================================================

    if ~ isempty(fileinfo(atoms_tmp_directory)) then
        rmdir(atoms_tmp_directory,"s");
    end

    // Update the dependencies of packages that use another version of packages
    // that have been installed
    // =========================================================================

    for i=1:size( result(:,1) , "*" )

        packages_out = atomsUpdateDeps([result(i,1) result(i,2)],section);

        if ATOMSVERBOSE then
            for j=1:size(packages_out(:,1),"*")
                atomsDisp(msprintf("\t%s (%s) will now use the version %s of the package %s",packages_out(j,1),packages_out(j,2),result(i,1),result(i,2)));
            end
        end

    end

    // Remove orphan packages
    // =========================================================================

    if section=="all" then
        sections = ["user";"allusers"];
    else
        sections = section;
    end

    for i=1:size(sections,"*")

        orphan_list = atomsOrphanList(sections(i));

        if ~ isempty(orphan_list) then
            atomsRemove( [ orphan_list(:,1) orphan_list(:,2) ] , sections(i) );
        end

    end

    // Go to the initial location
    // =========================================================================
    chdir(ATOMSINITIALPATH);

endfunction
