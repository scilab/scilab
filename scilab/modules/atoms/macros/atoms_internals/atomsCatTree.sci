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

// =============================================================================
// tree_out = atomsCatTree( tree_in_1 , tree_in_2 )
//
// Date : 19/05/2009
// =============================================================================

function tree_out = atomsCatTree( tree_in_1 , tree_in_2 )

    rhs = argn(2);

    // Check number of input arguments
    // =========================================================================

    if rhs <> 2 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsCatTree",1));
    end

    // Check input arguments type
    // =========================================================================

    if type(tree_in_1) <> 17 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: matrix oriented typed list expected.\n"),"atomsCatTree",1));
    end

    if type(tree_in_2) <> 17 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: matrix oriented typed list expected.\n"),"atomsCatTree",2));
    end

    // tree_in_1 is the master tree :
    // =========================================================================

    tree_out  = tree_in_1;

    // Loop on tree_in_2 fields
    // =========================================================================

    fields = getfield(1,tree_in_2);
    fields(1:2) = [];

    for i=1:size(fields,"*")
        if ~ isfield(tree_out,fields(i)) then
            tree_out(fields(i)) = tree_in_2(fields(i));
        end
    end

endfunction
