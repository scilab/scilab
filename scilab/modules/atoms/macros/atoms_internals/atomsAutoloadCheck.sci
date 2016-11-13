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

function result = atomsAutoloadCheck(packages,section)

    rhs        = argn(2);
    result     = []; // Column vector that contains autoloaded packages

    // Check write access on allusers zone
    // =========================================================================
    ATOMSALLUSERSWRITEACCESS = atomsAUWriteAccess();

    // Check number of input arguments
    // =========================================================================

    if rhs <> 2 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsAutoloadCheck",2));
    end

    // Check input parameters type
    // =========================================================================

    if type(packages) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsAutoloadCheck",1));
    end

    if (size(packages(1,:),"*") < 1) | (size(packages(1,:),"*") > 3) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: mx1, mx2 or mx3 string matrix expected.\n"),"atomsAutoloadCheck",1));
    end

    // Allusers/user management
    // =========================================================================

    if type(section) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsAutoloadCheck",2));
    end

    if and(section<>["user","allusers","all"]) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'', ''allusers'' or ''all'' expected.\n"),"atomsAutoloadCheck",2));
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

        result(i) = %F;

        if ~isempty(packages(i,2)) & ~isempty(packages(i,3)) then

            // Version : specified
            // Section : specified

            for j=1:size(autoloaded(:,1),"*")
                if and( packages(i,:) == autoloaded(j,:) ) then
                    result(i) = %T;
                    break;
                end
            end

        elseif ~isempty(packages(i,2)) & isempty(packages(i,3)) then

            // Version : specified
            // Section : It doesn't matter

            for j=1:size(autoloaded(:,1),"*")
                if and([packages(i,1) packages(i,2)] == [autoloaded(j,1) autoloaded(j,2)]) then
                    result(i) = %T;
                    break;
                end
            end

        elseif isempty(packages(i,2)) & ~isempty(packages(i,3)) then

            // Version : It doesn't matter
            // Section : specified

            for j=1:size(autoloaded(:,1),"*")
                if and([packages(i,1) packages(i,3)] == [autoloaded(j,1) autoloaded(j,3)]) then
                    result(i) = %T;
                    break;
                end
            end

        elseif isempty(packages(i,2)) & isempty(packages(i,3)) then

            // Version : It doesn't matter
            // Section : It doesn't matter

            result(i) = (find( packages(i,1) == autoloaded(:,1)) <> [])

        end
    end

endfunction
