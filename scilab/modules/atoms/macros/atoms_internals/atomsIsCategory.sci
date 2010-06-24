// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Internal function

// Return true if the input argument is a listed category
//        false otherwise

// atomsIsCategory(['Modeling and Control Tools' 'Neural networks'])

function result = atomsIsCategory(categories)

    // Initialize
    // =========================================================================
    result = [];

    // Check input parameters
    // =========================================================================

    rhs  = argn(2);

    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsIsCategory",1));
    end

    if type(categories) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsIsCategory",1));
    end

    if size(categories(1,:),"*") > 2 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: mx1 or mx2 string matrix expected.\n"),"atomsIsCategory",1));
    end

    if size(categories(1,:),"*") == 2 then
        categories = categories(:,1)+" - "+categories(:,2);
    end

    // Official categories
    // =========================================================================

    officialcategories = atomsCategoryGet();
    officialcategories = officialcategories(:,1)+" - "+officialcategories(:,2);
    officialcategories = strsubst(officialcategories,"/ - $/","","r");

    // loop on categories
    // =========================================================================

    for i=1:size(categories,"*")
        result(i) = or(categories(i) == officialcategories);
    end

endfunction
