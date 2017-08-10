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

// Remove toolboxes to the list of packages that are automatically loaded at Scilab start
// This function has an impact on the following files :
//  -> ATOMSDIR/autoloaded

// End-User function

function nbDel = atomsAutoloadDel(packages,section)

    // Load Atoms Internals lib if it's not already loaded
    // =========================================================================
    if ~ exists("atomsinternalslib") then
        load("SCI/modules/atoms/macros/atoms_internals/lib");
    end

    // Check write access on allusers zone
    // =========================================================================
    ATOMSALLUSERSWRITEACCESS = atomsAUWriteAccess();

    rhs   = argn(2);
    nbDel = 0;

    // Check number of input arguments
    // =========================================================================

    if rhs < 1 | rhs > 2 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsAutoloadDel",1,2));
    end

    // Check input parameters type
    // =========================================================================

    if type(packages) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsAutoloadDel",1));
    end

    if (size(packages(1,:),"*") < 1) | (size(packages(1,:),"*") > 3) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: mx1, mx2 or mx3 string matrix expected.\n"),"atomsAutoloadDel",1));
    end

    // Allusers/user management
    //   - If Allusers is equal to "all", module(s) will removed from both "user"
    //     and "allusers" list
    //       → SCI/.atoms/autoloaded
    //       → SCIHOME/atoms/autoloaded
    //   - If Allusers is equal to "allusers", module(s) will removed from the
    //     "allusers" list
    //       → SCI/.atoms/autoloaded
    //   - If Allusers is equal to "user", module(s) will removed from the
    //     "user" list
    //       → SCIHOME/atoms/autoloaded
    // =========================================================================

    if rhs < 2 then

        if ATOMSALLUSERSWRITEACCESS then
            section = "all";
        else
            section = "user";
        end

    else
        // Process the 2nd input argument : allusers
        // Allusers can equal to "user","allusers" or "all"

        if (type(section) <> 4) & (type(section) <> 10) then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Boolean or single string expected.\n"),"atomsAutoloadDel",2));
        end

        if (type(section) == 10) & and(section<>["user","allusers","all"]) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'',''allusers'' or ''all'' expected.\n"),"atomsAutoloadDel",2));
        end

        // Check if we have the write access
        if or(section==["all","allusers"]) & ~ ATOMSALLUSERSWRITEACCESS then
            error(msprintf(gettext("%s: You haven''t write access on this directory : %s.\n"),"atomsAutoloadDel",pathconvert(SCI+"/.atoms")));
        end
    end

    if section == "all" then
        sections = ["user","allusers"];
    else
        sections = section;
    end

    // Complete packages matrix with empty columns
    // =========================================================================

    if size(packages(1,:),"*") == 1 then
        packages = [ packages emptystr(size(packages(:,1),"*"),1) emptystr(size(packages(:,1),"*"),1) ];

    elseif size(packages(1,:),"*") == 2 then
        packages = [ packages emptystr(size(packages(:,1),"*"),1) ];

    end

    // First filter : No need to process packages that are not present
    // =========================================================================
    packages(find(atomsAutoloadCheck(packages,section)==%F),:) = [];

    // Loop on each sections
    // =========================================================================

    for i=1:size(sections,"*")

        autoloaded = atomsAutoloadLoad(sections(i));

        for j=1:size(packages(:,1),"*")

            if ~isempty(packages(j,2)) & ~isempty(packages(j,3)) then

                // Version : specified
                // Section : specified

                del_index = [];

                for k=1:size(autoloaded(:,1),"*")
                    if and( packages(j,:) == autoloaded(k,:) ) then
                        nbDel     = nbDel + 1;
                        del_index = [ del_index k ];
                    end
                end

                autoloaded(del_index,:) = [];

            elseif ~isempty(packages(j,2)) & isempty(packages(j,3)) then

                // Version : specified
                // Section : It doesn't matter

                del_index = [];

                for k=1:size(autoloaded(:,1),"*")
                    if and([packages(j,1) packages(j,2)] == [autoloaded(k,1) autoloaded(k,2)]) then
                        nbDel     = nbDel + 1;
                        del_index = [ del_index k ];
                    end
                end

                autoloaded(del_index,:) = [];

            elseif isempty(packages(j,2)) & ~isempty(packages(j,3)) then

                // Version : It doesn't matter
                // Section : specified

                del_index = [];

                for k=1:size(autoloaded(:,1),"*")
                    if and([packages(j,1) packages(j,3)] == [autoloaded(k,1) autoloaded(k,3)]) then
                        nbDel     = nbDel + 1;
                        del_index = [ del_index k ];
                    end
                end

                autoloaded(del_index,:) = [];

            elseif isempty(packages(j,2)) & isempty(packages(j,3)) then

                // Version : It doesn't matter
                // Section : It doesn't matter

                del_index = [];

                for k=1:size(autoloaded(:,1),"*")
                    if packages(j,1) == autoloaded(k,1) then
                        nbDel     = nbDel + 1;
                        del_index = [ del_index k ];
                    end
                end

                autoloaded(del_index,:) = [];

            end

        end

        atomsAutoloadSave(autoloaded,sections(i));

    end

endfunction
