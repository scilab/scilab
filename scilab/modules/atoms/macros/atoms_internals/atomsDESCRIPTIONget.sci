// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

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

    packages_path      = atomsPath("system","user") + "packages";
    categories_path    = atomsPath("system","user") + "categories";
    packages_path_info = fileinfo(packages_path);

    // If necessary, rebuild the struct
    // =========================================================================

    if (packages_path_info == []) ..
        | (getdate("s") - packages_path_info(6) > 3600) ..
        | (rhs == 1 & update) then

        // Initialize
        packages         = struct();
        categories_flat  = struct();
        categories       = struct();

        description                    = struct();
        description("packages")        = packages;
        description("categories")      = categories;
        description("categories_flat") = categories_flat;

        // Operating system detection + Architecture detection
        // =========================================================================
        [OSNAME,ARCH,LINUX,MACOSX,SOLARIS,BSD] = atomsGetPlatform();

        description_files = [ ..
            atomsPath("system","allusers") + "DESCRIPTION_installed" "" ; ..
            atomsPath("system","user")     + "DESCRIPTION_installed" "" ; ..
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

        for i=1:size(repositories,"*")

            // Building url & file_out
            // ----------------------------------------
            url            = repositories(i)+"/TOOLBOXES/"+ARCH+"/"+OSNAME+".gz";
            file_out       = pathconvert(atoms_tmp_directory+string(i)+"_TOOLBOXES.gz",%f);

            // Remove the existing file
            // ----------------------------------------
            if fileinfo(file_out) <> [] then
                mdelete(file_out);
            end

            // Launch the download
            // ----------------------------------------
            atomsDownload(url,file_out);

            // Extract It
            // ----------------------------------------

            if LINUX | MACOSX | SOLARIS | BSD then
                extract_cmd = "gunzip "+ file_out;

            else
                extract_cmd = getshortpathname(pathconvert(SCI+"/tools/gzip/gzip.exe",%F)) + " -d """ + file_out + """";
            end

            [rep,stat,err] = unix_g(extract_cmd);

            if stat ~= 0 then
                error(msprintf(gettext("%s: Extraction of the DESCRIPTION file (''%s'') has failed.\n"),"atomsDESCRIPTIONget",file_out));
            end

            description_files = [ description_files ; strsubst(file_out,"/\.gz$/","","r") repositories(i) ];

        end

        // 2nd step : Loop on available Description files
        // ---------------------------------------------------------------------

        for i=1:size(description_files(:,1),"*")

            file_out        = description_files(i,1);
            this_repository = description_files(i,2);

            if isempty(fileinfo(file_out)) then
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

        packages         = description("packages");
        categories       = description("categories");
        categories_flat  = description("categories_flat");

        save(packages_path,packages,categories,categories_flat)

    // Just load from file
    // =========================================================================

    else
        load(packages_path,"packages","categories","categories_flat");
    end

endfunction
