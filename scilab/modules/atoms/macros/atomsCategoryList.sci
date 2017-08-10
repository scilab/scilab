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

// End user function

// Display the list of categories

function atomsCategoryList(section)

    // Load Atoms Internals lib if it's not already loaded
    // =========================================================================
    if ~ exists("atomsinternalslib") then
        load("SCI/modules/atoms/macros/atoms_internals/lib");
    end

    rhs            = argn(2);
    categories_mat = [];

    // Check number of input arguments
    // =========================================================================

    if rhs > 1 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsCategoryList",0,1));
    end

    // Check input argument type (if any)
    // =========================================================================

    if (rhs==1) & (type(section) <> 10) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsCategoryList",1));
    end

    // Check input argument dimension (if any)
    // =========================================================================

    if (rhs==1) & (size(section,"*")<>1) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: string expected.\n"),"atomsCategoryList",1));
    end

    // Check input argument values (if any)
    // =========================================================================

    if (rhs==1) & (and(section<>["all","main"])) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: ''all'' or ''main'' expected.\n"),"atomsCategoryList",1));
    end

    // Default value of section
    // =========================================================================

    if rhs==0 then
        section = "all";
    end

    // Get the categories matrix
    // =========================================================================

    categories = atomsCategoryGet("filter:"+section);

    for i=1:size(categories(:,1),"*")
        if size(categories(i,:),"*")==1 | categories(i,2)=="" then
            mprintf("%s\n",categories(i,1));
        else
            mprintf("%s - %s\n",categories(i,1),categories(i,2));
        end
    end

endfunction
