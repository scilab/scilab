// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
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
