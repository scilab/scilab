// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Single string expected.\n"),"atomsCategoryList",1));
    end

    // Check input argument dimension (if any)
    // =========================================================================

    if (rhs==1) & (size(section,"*")<>1) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: Single string expected.\n"),"atomsCategoryList",1));
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
