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

// Input parameters
//
// - categoryFilter:
//   + it can be a valid category
//   + "filter:all"
//   + "filter:main"

// Returns the list of available categories in ascending (lexical) order.

//       -->atomsCategoryGet('Optimization')
//        ans  =
//
//       !Derivative-free  !
//       !                 !
//       !Linear           !

//       -->atomsCategoryGet('filter:all')
//        ans  =
//
//       !Data Acquisition                               !
//       !                                               !
//       !Data Analysis And Statistics                   !
//       !                                               !
//       !Data Analysis And Statistics  Neural networks  !
//       !                                               !
//       !Data Handling                                  !
//       !                                               !
//       ....

//       -->atomsCategoryGet('filter:main')
//        ans  =
//
//       !Data Acquisition              !
//       !                              !
//       !Data Analysis And Statistics  !
//       !                              !
//       !Data Handling                 !
//       !                              !
//       ....

function categories_mat = atomsCategoryGet(categoryFilter)

    rhs            = argn(2);
    categories_mat = [];

    // Check number of input arguments
    // =========================================================================

    if rhs > 1 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsCategoryGet",0,1));
    end

    // Check input argument type (if any)
    // =========================================================================

    if (rhs==1) & (type(categoryFilter) <> 10) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsCategoryGet",1));
    end

    // Check input argument dimension (if any)
    // =========================================================================

    if (rhs==1) & (size(categoryFilter,"*")<>1) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: string expected.\n"),"atomsCategoryGet",1));
    end

    // Check input argument values (if any)
    // =========================================================================

    if (rhs==1) ..
        & (and(categoryFilter<>["filter:all","filter:main"])) ..
        & ( (regexp(categoryFilter,"/ - /")<>[]) | ~atomsIsCategory(categoryFilter)) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: ''filter:all'',''filter:main'' or a valid main category expected.\n"),"atomsCategoryGet",1));
    end

    // Default value of categoryFilter
    // =========================================================================

    if rhs==0 then
        categoryFilter = "filter:all";
    end

    // Get the main categories structure
    // =========================================================================

    [packages,categories_flat,categories] = atomsDESCRIPTIONget();

    if or(categoryFilter == ["filter:all","filter:main"]) then

        categories_mat      = getfield(1,categories);
        categories_mat(1:2) = [];
        categories_mat      = gsort(categories_mat,"lc","i");
        categories_mat      = categories_mat';

        if categoryFilter == "filter:all" then

            main_mat       = categories_mat;
            categories_mat = [];

            for i=1:size(main_mat,"*")
                categories_mat = [ categories_mat ; main_mat(i) "" ];
                sub_categories = gsort(categories(main_mat(i)),"lr","i");
                if sub_categories <> [] then
                    categories_mat = [ categories_mat ; emptystr(size(sub_categories,"*"),1)+main_mat(i) sub_categories ];
                end
            end
        end

    else
        categories_mat = gsort(categories(categoryFilter),"lr","i");
    end

endfunction
