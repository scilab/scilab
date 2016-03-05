// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2006 - INRIA - Farid BELAHCENE
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function tree=sci_legendre(tree)
    // M2SCI function
    // Conversion function for Matlab legendre()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree
    // Emulation function: mtlb_legendre()

    tree.name = "mtlb_legendre";
    boolknowndims = %t;
    dims = list();
    for i=1:size(tree.rhs(2).dims)
        if tree.rhs(2).dims(i) == -1 then
            boolknowndims = %f;
            break
        end
        dims($+1) = tree.rhs(2).dims(i);
    end

    if boolknowndims then
        if typeof(tree.rhs(1)) == "cste"
            if tree.rhs(1).value > 0 then
                dims = list(tree.rhs(1).value+1,dims)
            end
        else
            dims = list(-1,-1)
        end
    else
        dims = list(-1,-1)
    end

    tree.lhs(1).dims = dims;
    tree.lhs(1).type = Type(Double,Real);

endfunction
