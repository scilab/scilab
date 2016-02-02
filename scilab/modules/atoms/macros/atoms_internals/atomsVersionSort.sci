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

// Tri un tableau de versions

function versions_out = atomsVersionSort( versions_in , direction )

    rhs = argn(2);

    // Check number of input arguments
    // =========================================================================

    if rhs <> 2 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsVersionSort",2));
    end

    // if versions_in == [], no need to continue
    // =========================================================================

    if versions_in == [] then
        versions_out = [];
        return;
    end

    // Check input parameters type
    // =========================================================================

    if type(versions_in) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsVersionSort",1));
    end

    if type(direction) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsVersionSort",2));
    end

    // Check input parameters dimension
    // =========================================================================

    if size(direction,"*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: string expected.\n"),"atomsVersionSort",2));
    end

    // Check input parameters value
    // =========================================================================
    if and(direction<>["ASC";"DESC"]) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: ""ASC"" or ""DESC"" expected.\n"),"atomsVersionSort",2));
    end

    // Output argument
    // =========================================================================
    versions_out = versions_in;

    ASC  = (convstr(direction,"u") == "ASC");
    DESC = ~ ASC;

    // Now : action
    // =========================================================================

    for i=1:(size(versions_out,"*")-1)
        for j=1:(size(versions_out,"*")-1)
            if ASC & (atomsVersionCompare(versions_out(j),versions_out(j+1)) > 0) then
                tmp               = versions_out(j+1);
                versions_out(j+1) = versions_out(j);
                versions_out(j)   = tmp;
            elseif DESC & (atomsVersionCompare(versions_out(j),versions_out(j+1)) < 0) then
                tmp               = versions_out(j+1);
                versions_out(j+1) = versions_out(j);
                versions_out(j)   = tmp;
            end
        end
    end

    // Return a colum vector
    // =========================================================================
    versions_out = matrix(versions_out,-1,1);

endfunction
