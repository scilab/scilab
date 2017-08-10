// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [tree]=%i2sci_r(tree)
    // M2SCI function
    // Conversion function for Matlab insertion in row vectors (called by %i2sci())
    // Input: tree = Matlab operation tree
    // Output: tree = Scilab equivalent for tree

    from=tree.operands($)
    to=tree.operands(1)
    ind=tree.operands(2)

    if from.dims(1)==1 & from.dims(2)==1 then // Insert a scalar
    elseif from.dims(1)==1 then // Insert a row vector
    elseif from.dims(1)<>Unknown & from.dims(2)<>Unknown then // Insert a matrix with known sizes
        tree.operands($)=Funcall("matrix",1,Rhs_tlist(from,1,Operation("-",list(Cste(1)),list())))
    else
        if ~isdefinedvar(from) then
            w=gettempvar()
            m2sci_insert(Equal(list(w),from))
        else
            w=from
        end

        // from=from(:).'
        tmp=Operation("ext",list(w,Cste(":")),list())
        tmp=Operation(".''",list(tmp),list())

        tree.operands(4)=tmp
        tree.operands(3)=ind
        tree.operands(2)=Cste(1)
    end

    // Data inference
    tree.out(1).dims=list(1,Unknown)
    tree.out(1).type=to.type
endfunction


