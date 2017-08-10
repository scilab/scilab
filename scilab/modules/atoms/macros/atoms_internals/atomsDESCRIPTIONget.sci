// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
// Copyright (C) 2011-2012 - DIGITEO - Allan CORNET
// Copyright (C) 2012 - Samuel GOUGEON
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

// Return the full description of the packages present in the differents
// repositories and the installed packages

function [packages,categories_flat,categories] = atomsDESCRIPTIONget(update)

    // Check input parameters
    // =========================================================================

    rhs  = argn(2);

    if rhs > 1 then
        error(msprintf(gettext("%s: Wrong number of input argument: at most %d expected.\n"),"atomsDESCRIPTIONget",1));
    end

    if (rhs == 1) & (type(update) <> 4) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Boolean expected.\n"),"atomsDESCRIPTIONget",1));
    end

    // packages file path definition
    // =========================================================================
    atoms_path_all_users = atomsPath("system", "allusers");
    atoms_path_user = atomsPath("system", "user");

    filename_test_rw = "__ATOMS_RW__";
    if isfile(atoms_path_all_users + filename_test_rw) then
        mdelete(atoms_path_all_users + filename_test_rw);
    end

    // Have the user write access to the Scilab distribution ?
    [id, err] = mopen(atoms_path_all_users + filename_test_rw, "wt");
    if err == 0 then // The user has write access to the Scilab distribution
        mclose(id);
        mdelete(atoms_path_all_users + filename_test_rw);

        packages_path = atoms_path_all_users + "packages";
        packages_path_user = atoms_path_user + "packages";

        // Then, we must test if there is also a user defined packages file
        if isfile(packages_path_user) then  // We use it if it is newer
            kdiff = newest([packages_path, packages_path_user])
            if ~isfile(packages_path) | kdiff == 2 then
                copyfile(packages_path_user, packages_path)
            end
        end
    else  // The user is not a Scilab admin => default user profile used
        packages_path = atoms_path_user + "packages";
    end

    categories_path  = atoms_path_user + "categories";
    packages_path_info = fileinfo(packages_path);


    // in offline mode we use only DESCRIPTION file of the module
    if (atomsGetConfig("offLine") == "True" | atomsGetConfig("offline") == "True") then
        if isfile(atomsPath("system", "session") + "/DESCRIPTION_archives") then
            this_description = atomsDESCRIPTIONread(atomsPath("system","session") + "/DESCRIPTION_archives");

            inst_description_files = [ ..
            atomsPath("system","allusers") + "DESCRIPTION_installed" "" ; ..
            atomsPath("system","user")   + "DESCRIPTION_installed" "" ; ..
            atomsPath("system","session")  + "DESCRIPTION_installed" "" ; ..
            atomsPath("system","session")  + "DESCRIPTION_archives"  "" ];

            for i=1:size(inst_description_files(:,1),"*")

                file_out    = inst_description_files(i,1);
                this_repository = inst_description_files(i,2);

                if ~isfile(file_out) then
                    continue;
                end

                // Read the download description file
                // ----------------------------------------
                if this_repository <> "" then
                    additional("repository") = this_repository;
                else
                    additional = struct();
                end

                installed_description = atomsDESCRIPTIONread(file_out,additional);

                this_description  = atomsDESCRIPTIONcat(installed_description,this_description);

            end

            packages = this_description("packages");
            categories_flat = this_description("categories_flat");
            categories = this_description("categories");

            global %_atoms_cache; // /!\ Do not rename this variable. Name is tracked/ignored by Variable Browser
            %_atoms_cache(1) = packages;
            %_atoms_cache(2) = categories;
            %_atoms_cache(3) = categories_flat;

        else
            load(packages_path, "packages", "categories", "categories_flat");

        end

        return
    end


    // If necessary, rebuild the struct
    // =========================================================================
    TIME_BEFORE_NEW_UPDATE = strtod(atomsGetConfig("updateTime"))*86400;
    if isnan(TIME_BEFORE_NEW_UPDATE) then // Value not found in config file, give the default one
        TIME_BEFORE_NEW_UPDATE = 86400*30; // One month, in seconds
    end
    if (packages_path_info == []) ..
        | (getdate("s") - packages_path_info(6) > TIME_BEFORE_NEW_UPDATE) ..
        | (rhs == 1 & update) then

        // Initialize
        packages     = struct();
        categories_flat  = struct();
        categories     = struct();

        description          = struct();
        description("packages")    = packages;
        description("categories")    = categories;
        description("categories_flat") = categories_flat;

        // Operating system detection + Architecture detection
        // =========================================================================
        [OSNAME, ARCH, LINUX, MACOSX, SOLARIS, BSD] = atomsGetPlatform();

        description_files = [ ..
        atomsPath("system","allusers") + "DESCRIPTION_installed" "" ; ..
        atomsPath("system","user")   + "DESCRIPTION_installed" "" ; ..
        atomsPath("system","session")  + "DESCRIPTION_installed" "" ; ..
        atomsPath("system","session")  + "DESCRIPTION_archives"  "" ];

        // 1st step : Loop on available repositories
        // ---------------------------------------------------------------------

        repositories = atomsRepositoryList();
        repositories = repositories(:,1);

        atoms_tmp_directory = atomsPath("system","session");

        if ~isdir(atoms_tmp_directory) then
            mkdir(atoms_tmp_directory);
        end

        nbRepoError = 0;
        allRepoError = [];
        for i=1:size(repositories,"*")
            // Building url & file_out
            // ----------------------------------------
            url      = repositories(i)+"/TOOLBOXES/"+ARCH+"/"+OSNAME+".gz";
            file_out     = pathconvert(getshortpathname(fullpath(atoms_tmp_directory))+string(i)+"_TOOLBOXES.gz",%f);

            // Remove the existing file
            // ----------------------------------------
            if fileinfo(file_out) <> [] then
                mdelete(file_out);
            end

            // Launch the download
            // ----------------------------------------
            try
                atomsDownload(url, file_out);

                // We check that file_out exists
                // ----------------------------------------
                if ~isfile(file_out) then
                    error(msprintf(gettext("%s: DESCRIPTION file (''%s'') does not exist.\n"),"atomsDESCRIPTIONget", file_out));
                end

                // Extract It
                // ----------------------------------------
                if LINUX | MACOSX | SOLARIS | BSD then
                    extract_cmd = "gunzip "+ file_out;
                else
                    gzip_path = getshortpathname(fullpath(pathconvert(SCI+"/tools/gzip/gzip.exe",%F)));
                    if ~isfile(gzip_path) then
                        error(msprintf(gettext("%s: gzip not found.\n"), "atomsDESCRIPTIONget"));
                    end
                    extract_cmd = """" + gzip_path + """" + " -d """ + file_out + """";
                end

                [rep, stat ,err] = unix_g(extract_cmd);

                if stat ~= 0 then
                    disp(err);
                    error(msprintf(gettext("%s: Extraction of the DESCRIPTION file (''%s'') has failed.\n"),"atomsDESCRIPTIONget",file_out));
                end

                description_files = [ description_files ; strsubst(file_out,"/\.gz$/","","r") repositories(i) ];
                mprintf(_("Scanning repository") + " " + repositories(i) + " ... " + _("Done") + "\n\n");
            catch
                [msg, ierr] = lasterror();
                nbRepoError = nbRepoError + 1;
                allRepoError = [allRepoError ; msg]
                // If failed downloading latest repository and all previous failed => error
                if nbRepoError == size(repositories, "*") then
                    warning(msg);
                    mprintf(_("Scanning repository") + " " + repositories(i) + " ... " + _("Skipped") + "\n\n");
                    error("All ATOMS repositories scan failed.");
                else
                    warning(msg);
                    mprintf(_("Scanning repository") + " " + repositories(i) + " ... " + _("Skipped") + "\n\n");
                end
            end
        end

        // 2nd step : Loop on available Description files
        // ---------------------------------------------------------------------

        for i=1:size(description_files(:,1),"*")

            file_out    = description_files(i,1);
            this_repository = description_files(i,2);

            if ~isfile(file_out) then
                continue;
            end

            // Read the download description file
            // ----------------------------------------
            if this_repository <> "" then
                additional("repository") = this_repository;
            else

                additional = struct();
            end

            this_description = atomsDESCRIPTIONread(file_out,additional);

            // Add information about the repository
            // ----------------------------------------
            if this_repository <> "" then
                this_description = atomsDESCRIPTIONaddField(this_description,"*","*","repository",this_repository);
                this_description = atomsDESCRIPTIONaddField(this_description,"*","*","fromRepository","1");
            end

            // concatenate the description with the
            // global struct
            // ----------------------------------------
            description  = atomsDESCRIPTIONcat(description,this_description);

            // file_out is no more needed, delete it
            // but only if it's named TOOLBOXES
            // DESCRIPTION_installed must be keeped
            // ----------------------------------------
            if regexp( file_out , "/TOOLBOXES$/" , "o" ) <> [] then
                mdelete(file_out);
            end

        end

        // Save the "packages" variable in a file
        // ---------------------------------------------------------------------

        if ~isdir(atomsPath("system","user")) then
            mkdir(atomsPath("system","user"));
        end

        packages     = description("packages");
        categories     = description("categories");
        categories_flat  = description("categories_flat");

        commandToExec = "save(packages_path, ""packages"", ""categories"", ""categories_flat"")";
        ierr = execstr(commandToExec, "errcatch");
        if ierr <> 0 then
            error(msprintf(gettext("%s: save (''%s'') has failed.\n"),"atomsDESCRIPTIONget", packages_path));
        end
        clearglobal %_atoms_cache;

        // Just load from file
        // =========================================================================

    else
        global %_atoms_cache; // /!\ Do not rename this variable. Name is tracked/ignored by Variable Browser
        if isempty(%_atoms_cache) then
            load(packages_path,"packages","categories","categories_flat");
            %_atoms_cache = list(packages, categories, categories_flat);
        else
            packages = %_atoms_cache(1);
            categories = %_atoms_cache(2);
            categories_flat = %_atoms_cache(3);
        end
    end

endfunction
