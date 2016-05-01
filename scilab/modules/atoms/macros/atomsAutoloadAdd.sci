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

// Add toolboxes to the list of packages that are automatically loaded at Scilab start
// This function has an impact on the following files :
//  -> ATOMSDIR/autoloaded

// End-User function

function nbAdd = atomsAutoloadAdd(packages,section)

    rhs            = argn(2);
    nbAdd          = 0;
    autoloaded     = []; // Column vector that contains autoloaded packages

    // Load Atoms Internals lib if it's not already loaded
    // =========================================================================
    if ~ exists("atomsinternalslib") then
        load("SCI/modules/atoms/macros/atoms_internals/lib");
    end

    // Check write access on allusers zone
    // =========================================================================
    ATOMSALLUSERSWRITEACCESS = atomsAUWriteAccess();

    // Check number of input arguments
    // =========================================================================

    if rhs < 1 | rhs > 2 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsAutoloadAdd",1,2));
    end

    // Check input parameters type
    // =========================================================================

    if type(packages) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsAutoloadAdd",1));
    end

    if (size(packages(1,:),"*") < 1) | (size(packages(1,:),"*") > 3) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: mx1, mx2 or mx3 string matrix expected.\n"),"atomsAutoloadAdd",1));
    end

    // Allusers/user management
    //   - If section is equal to "allusers", module(s) will added to the
    //     "autoload" list of the "allusers" section :
    //       → SCI/.atoms/autoloaded
    //   - Otherwise, module(s) will added to the "autoload" list of the "user"
    //     section
    //       → SCIHOME/atoms/autoloaded
    // =========================================================================

    if rhs < 2 then

        // By default:
        //  → Add the module to the "autoload" list of the "allusers" section
        //    if we have the write access to SCI directory
        //  → Add the module to the "autoload" list of the "user" section otherwise

        if ATOMSALLUSERSWRITEACCESS then
            section = "allusers";
        else
            section = "user";
        end

    else

        // Process the 2nd input argument : allusers
        // Allusers can be equal to "user" or "allusers"

        if type(section) <> 10 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsAutoloadAdd",2));
        end

        if and(section<>["user","allusers"]) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'' or ''allusers'' expected.\n"),"atomsAutoloadAdd",2));
        end

        // Check if we have the write access
        if (section=="allusers") & ~ ATOMSALLUSERSWRITEACCESS then
            error(msprintf(gettext("%s: You haven''t write access on this directory : %s.\n"),"atomsAutoloadAdd",pathconvert(SCI+"/.atoms")));
        end

    end

    // Does the SCIHOME/atoms/autoloaded exist, if yes load it
    // =========================================================================
    autoloaded = atomsAutoloadLoad(section);

    // Complete packages matrix with empty columns
    // =========================================================================

    if size(packages(1,:),"*") == 1 then
        packages = [ packages emptystr(size(packages(:,1),"*"),1) emptystr(size(packages(:,1),"*"),1) ];

    elseif size(packages(1,:),"*") == 2 then
        packages = [ packages emptystr(size(packages(:,1),"*"),1) ];

    end

    // Loop on input parameter
    // =========================================================================

    for i=1:size(packages(:,1),"*")

        // The module's installed version hasn't been specified or is empty
        // → Set the MRV available
        // =====================================================================

        if isempty(packages(i,2)) then

            if ~ isempty(packages(i,3)) then
                searched_section = packages(i,3);

            elseif section=="user" then
                // User can add a module to its "user" autoload list even if it's
                // installed in the "allusers" section
                searched_section = "all";

            else
                // A module installed in the user section cannot be add in the
                // "autoload" list of all users
                searched_section = "allusers";

            end

            this_module_versions = atomsGetInstalledVers(packages(i,1),searched_section);

            if isempty(this_module_versions) then
                error(msprintf(gettext("%s: Module ''%s'' is not installed (''%s'' section).\n"),"atomsLoad",packages(i,1),searched_section));
            else
                packages(i,2) = this_module_versions(1);
            end

        else

            if ~atomsIsInstalled([packages(i,1) packages(i,2)]) then
                error(msprintf(gettext("%s: Module ''%s - %s'' is not installed.\n"),"atomsLoad",packages(i,1),packages(i,2)));
            end

            // If the packaging version is not mentioned, define it
            if isempty(strindex(packages(i,2),"-")) then
                this_package_details = atomsGetInstalledDetails(packages(i,1:3));
                packages(i,2)        = this_package_details(2);
            end

        end

        // The module's installed section hasn't been specified or is empty
        // =====================================================================

        if isempty(packages(i,3)) then

            if atomsIsInstalled([packages(i,1) packages(i,2)],section) then
                packages(i,3) = section;

            elseif section=="user" & atomsIsInstalled([packages(i,1) packages(i,2)],"allusers") then
                packages(i,3) = "allusers";

            elseif section=="allusers" & atomsIsInstalled([packages(i,1) packages(i,2)],"user") then
                mprintf(gettext("%s: The following module is installed in the user section, you cannot add it to the ""autoload"" list for all users:\n"),"atomsAutoloadAdd");
                printf(gettext("\t - ''%s - %s''\n"),packages(i,1),packages(i,2));
                error("");

            else
                mprintf(gettext("%s: The following module is not installed:\n"),"atomsAutoloadAdd");
                mprintf(gettext("\t - ''%s - %s''\n"),packages(i,1),packages(i,2));

            end

        else

            // Check if modules are installed
            if ~ atomsIsInstalled([packages(i,1) packages(i,2)],packages(i,3)) then
                mprintf(gettext("%s: The following modules are not installed:\n"),"atomsAutoloadAdd");
                mprintf("\t - ''%s - %s'' (''%s'' section)\n",packages(i,1),packages(i,2),packages(i,3));
                error("");
            end

            // A module installed in the user section cannot be add in the
            // "autoload" list of all users

            if (section=="allusers") & (packages(i,3)=="user") then
                mprintf(gettext("%s: The following module is installed in the user section, you cannot add it to the ""autoload"" list for all users:\n"),"atomsAutoloadAdd");
                mprintf(gettext("\t - ''%s - %s''\n"),packages(i,1),packages(i,2));
                error("");
            end

        end

        // Now we can really add it if it doesn't already exist in the list
        // =====================================================================

        if ~ atomsAutoloadCheck(packages(i,:),section) then
            nbAdd      = nbAdd + 1;
            autoloaded = [ autoloaded ; packages(i,:) ];
        end

    end

    // Apply changes
    // =========================================================================

    if nbAdd > 0 then
        atomsAutoloadSave(autoloaded,section);
    end

endfunction
