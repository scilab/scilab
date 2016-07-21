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

// Load one or several toolboxes

function result = atomsLoad(packages)

    // Load Atoms Internals lib if it's not already loaded
    // =========================================================================
    if ~ exists("atomsinternalslib") then
        load("SCI/modules/atoms/macros/atoms_internals/lib");
    end

    // Init the output argument
    // =========================================================================
    result = [];

    // Check ATOMSAUTOLOAD variable
    // =========================================================================
    if ~isdef("ATOMSAUTOLOAD") | (ATOMSAUTOLOAD<>%T) then
        ATOMSAUTOLOAD = %F;
    end

    // Check input parameters
    // =========================================================================

    rhs = argn(2);

    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),"atomsLoad",1,2))
    end

    if type(packages) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsLoad",1));
    end

    if size(packages(1,:),"*") > 3 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: mx1,mx2 or mx3 string matrix expected.\n"),"atomsLoad",1));
    end

    packages = stripblanks(packages);

    // Complete packages matrix with empty columns
    // =========================================================================

    if size(packages(1,:),"*") == 1 then
        packages = [ packages emptystr(size(packages(:,1),"*"),1) emptystr(size(packages(:,1),"*"),1) ];

    elseif size(packages(1,:),"*") == 2 then
        packages = [ packages emptystr(size(packages(:,1),"*"),1) ];

    end

    // Plan the path → 4th column
    // =========================================================================
    packages = [ packages emptystr(size(packages(:,1),"*"),1) ];

    // Verbose Mode ?
    // =========================================================================
    if strcmp(atomsGetConfig("Verbose"),"True", "i") == 0 then
        ATOMSVERBOSE = %T;
    else
        ATOMSVERBOSE = %F;
    end

    // Already loaded modules :
    // =========================================================================
    loaded = atomsLoadLoad();
    nbAdd  = 0;

    // Remove duplicate packages (name - version)
    // =========================================================================

    [matout,index] = unique([packages(:,1) packages(:,2)],"r");
    packages       = packages(gsort(index,"g","i"),:);

    // Loop on input parameter
    // =========================================================================

    for i=1:size(packages(:,1),"*")

        // The module's installed version hasn't been specified or is empty
        // → Set the MRV available
        // =====================================================================

        if isempty(packages(i,2)) then

            if ~ isempty(packages(i,3)) then
                section = packages(i,3);

            else
                section = "all";

            end

            this_module_versions = atomsGetInstalledVers(packages(i,1),section);

            if isempty(this_module_versions) then
                if section == "all" then
                    error(msprintf(gettext("%s: Module ''%s'' is not installed.\n"),"atomsLoad",packages(i,1)));
                else
                    error(msprintf(gettext("%s: Module ''%s'' is not installed (''%s'' section).\n"),"atomsLoad",packages(i,1),section));
                end
            else
                packages(i,2) = this_module_versions(1);
            end

        else

            if ~atomsIsInstalled([packages(i,1) packages(i,2)]) then
                error(msprintf(gettext("%s: Module ''%s - %s'' is not installed.\n"),"atomsLoad",packages(i,1),packages(i,2)));
            end

            // If the packaging version is not mentioned, define it
            if isempty(strindex(packages(i,2),"-")) then
                this_package_details = atomsGetInstalledDetails([packages(i,1) packages(i,2) packages(i,3)]);
                packages(i,2)        = this_package_details(2);
            end

        end

        // The module's installed section hasn't been specified or is empty
        // → If the module (same name/same version) is installed in both sections,
        //   module installed in the "user" section is taken
        // =====================================================================

        if isempty(packages(i,3)) then

            sections = ["user","allusers"];

            for j=1:size(sections,"*")
                if atomsIsInstalled([packages(i,1) packages(i,2)],sections(j)) then
                    packages(i,3) = sections(j);
                    break
                end
            end

        else

            // Check if modules are installed
            if ~ atomsIsInstalled([packages(i,1) packages(i,2)],packages(i,3)) then
                mprintf(gettext("%s: The following modules are not installed:\n"),"atomsAutoloadAdd");
                mprintf("\t - ''%s - %s'' (''%s'' section)\n",packages(i,1),packages(i,2),packages(i,3));
                error("");
            end

        end

        // Get the installed path
        // =====================================================================
        packages(i,4) = atomsGetInstalledPath([packages(i,1) packages(i,2) packages(i,3)]);

    end

    // Loop on packages gived by the user
    // =========================================================================

    mandatory_packages      = struct();
    mandatory_packages_name = struct();
    mandatory_packages_mat  = [];

    for i=1:size(packages(:,1),"*")

        this_package_name    = packages(i,1);
        this_package_version = packages(i,2);
        this_package_section = packages(i,3);
        this_package_path    = packages(i,4);

        // Check if the user try to load 2 versions of the same toolbox at the
        // same time
        // =====================================================================
        if size( find( this_package_name == packages(:,1) ) > 1 ) then
            this_versions = packages( find( this_package_name == packages(:,1) ) , 2 );
            for j=2:size(this_versions,"*")
                if this_versions(j) <> this_versions(1) then
                    mprintf(gettext("%s: Several versions of a package (%s) cannot be loaded at the same scilab session :\n"),"atomsLoad",this_package_name);
                    mprintf(gettext("\t - You''ve asked ''%s - %s''\n"),this_package_name,this_versions(1));
                    mprintf(gettext("\t - You''ve asked ''%s - %s''\n"),this_package_name,this_versions(j));
                    mprintf("\n");

                    if ATOMSAUTOLOAD then
                        return;
                    else
                        error("");
                    end
                end
            end
        end

        // Check if this toolbox is already loaded
        // =====================================================================
        if atomsIsLoaded([this_package_name this_package_version]) then
            atomsDisp(msprintf("\tThe package %s (%s) is already loaded",this_package_name,this_package_version));
            continue;
        end

        // Check if another version of this toolbox is already loaded
        // =====================================================================
        [is_loaded,loaded_version] =  atomsIsLoaded(this_package_name);
        if is_loaded then
            if ATOMSAUTOLOAD then
                mprintf(gettext("%s: Another version of the package %s is already loaded : %s\n"),"atomsLoad",this_package_name,loaded_version);
            else
                error(msprintf(gettext("%s: Another version of the package %s is already loaded : %s\n"),"atomsLoad",this_package_name,loaded_version));
            end
            continue;
        end

        mandatory_packages(this_package_name+" - "+this_package_version) = "asked_by_user";
        mandatory_packages_name(this_package_name) = this_package_version;
        mandatory_packages_mat = [ mandatory_packages_mat ; this_package_name this_package_version this_package_section this_package_path ];

    end

    // Fill the list of package to load
    // =========================================================================

    for i=1:size(packages(:,1),"*")

        this_package_name    = packages(i,1);
        this_package_version = packages(i,2);
        this_package_section = packages(i,3);
        this_package_path    = packages(i,4);

        childs = atomsGetDepChilds([this_package_name this_package_version]);

        for j=1:size( childs(:,1) , "*")

            // Check if it is already loaded
            // -------------------------------------------------------
            if atomsIsLoaded(childs(j,:)) then
                continue;
            end

            // Check if another version of this package is already loaded
            // -------------------------------------------------------
            [is_loaded,loaded_version] =  atomsIsLoaded(childs(j,1));
            if is_loaded then
                mprintf(gettext("%s: Several versions of a package (%s) cannot be loaded at the same scilab session :\n"),"atomsLoad",childs(j,1));
                mprintf(gettext("\t - ''%s - %s'' is already loaded\n"),childs(j,1),loaded_version);
                mprintf(gettext("\t - ''%s - %s'' is needed by ''%s - %s''\n"),childs(j,1),childs(j,2),this_package_name,this_package_version);
                mprintf("\n");

                if ATOMSAUTOLOAD then
                    return;
                else
                    error("");
                end
            end

            // Check if it is already in the list
            // -------------------------------------------------------
            if isfield( mandatory_packages , childs(j,1)+" - "+childs(j,2) ) then
                continue;
            end

            // Check if another version is already in the list
            // -------------------------------------------------------
            if isfield( mandatory_packages_name , childs(j,1) ) then

                // if it's not the name version => error
                if mandatory_packages_name(childs(j,1)) <> childs(j,2) then

                    mprintf(gettext("%s: Several versions of a package (%s) cannot be loaded at the same scilab session :\n"),"atomsLoad",childs(j,1));
                    mprintf(gettext("\t - ''%s - %s'' is needed by ''%s - %s''\n"),childs(j,1),childs(j,2),packages(i,1),packages(i,2));

                    // The other version of the package is asked by the user
                    if mandatory_packages(childs(j,1)+" - "+mandatory_packages_name(childs(j,1))) == "asked_by_user" then
                        mprintf(gettext("\t - You''ve asked ''%s - %s''\n"),childs(j,1),mandatory_packages_name(childs(j,1)));

                        // The other version of the package is a need by another package
                    else
                        mprintf(gettext("\t - ''%s - %s'' is needed by ''%s''\n"), ..
                        childs(j,1), .. // name
                        mandatory_packages_name(childs(j,1)), .. // version
                        mandatory_packages(childs(j,1)+" - "+mandatory_packages_name(childs(j,1))) .. // name - version
                        );
                    end

                    mprintf("\n");
                    if ATOMSAUTOLOAD then
                        return;
                    else
                        error("");
                    end
                end
            end

            // The child has passed the test, add it to the mandatory
            // packages to load
            // -------------------------------------------------------

            mandatory_packages(childs(j,1)+" - "+childs(j,2)) = packages(i,1)+" - "+packages(i,2);
            mandatory_packages_name(childs(j,1)) = childs(j,2);
            mandatory_packages_mat = [ childs(j,1) childs(j,2) this_package_section atomsGetInstalledPath(childs(j,:),this_package_section) ; mandatory_packages_mat ];
        end
    end

    // Libraries to resume
    // =========================================================================
    libs_resume = [];

    if ~ isempty(mandatory_packages_mat) then
        mprintf("\n");
    end

    for i=1:size(mandatory_packages_mat(:,1),"*")

        this_package_name    = mandatory_packages_mat(i,1);
        this_package_version = mandatory_packages_mat(i,2);
        this_package_section = mandatory_packages_mat(i,3);
        this_package_path    = mandatory_packages_mat(i,4);

        // Get the list of lib
        // =====================================================================
        libs_before = librarieslist();

        // Exec the loader
        // =====================================================================

        loader_file = pathconvert(this_package_path) + "loader.sce";

        if fileinfo(loader_file)==[] then
            msg = _("%s: The file ''%s'' from (%s - %s) does not exist or is not read accessible.\n")
            mprintf(msg, "atomsLoad", loader_file, this_package_name, this_package_version);
            if ATOMSAUTOLOAD then
                return;
            end
        end

        ierr = execstr("exec(loader_file,-1)","errcatch");

        if ierr > 0 then
            mprintf(gettext("%s: An error occurred while loading ''%s-%s'':\n"),"atomsLoad",this_package_name,this_package_version);
            errormsg = lasterror(%T);
            mprintf("\t%s\n",errormsg);
            if ATOMSAUTOLOAD then
                continue;
            else
                error("");
            end
        end

        mprintf("\n");

        // Get the list of libraries (macros)
        // =====================================================================
        libs_after = librarieslist();

        // Loop on libs_after
        // =====================================================================
        for j=1:size(libs_after,"*")

            if find(libs_after(j) == libs_before) == [] then
                libs_resume = [ libs_resume ; libs_after(j) ];
            end
        end

        // Fill the output argument
        // =====================================================================
        result = [ result ; mandatory_packages_mat(i,:) ];

        // fill the loaded matrix
        // =====================================================================

        if and(loaded(:,1) <> this_package_name) then
            nbAdd  = nbAdd + 1;
            loaded = [ loaded ; this_package_name this_package_version this_package_section ];
        end

    end

    // Apply changes
    // =========================================================================
    if nbAdd > 0 then
        atomsLoadSave(loaded);
    end

    // If libs_resume is empty, the job is done
    // =========================================================================
    if isempty(libs_resume) then
        return;
    end

    // Build the resume cmd
    // =========================================================================

    resume_cmd = "[";

    for i=1:size(libs_resume,"*")
        resume_cmd = resume_cmd + libs_resume(i);
        if i<size(libs_resume,"*") then
            resume_cmd = resume_cmd + ",";
        else
            resume_cmd = resume_cmd + "] = resume(";
        end
    end

    for i=1:size(libs_resume,"*")
        resume_cmd = resume_cmd + libs_resume(i);
        if i<size(libs_resume,"*") then
            resume_cmd = resume_cmd + ",";
        else
            resume_cmd = resume_cmd + ");";
        end
    end

    // Exec the resume cmd
    // =========================================================================
    execstr(resume_cmd);

endfunction
