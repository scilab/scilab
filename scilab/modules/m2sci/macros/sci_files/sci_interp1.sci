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

function   tree=sci_interp1(tree)
    // M2SCI function
    // Conversion function for Matlab interp1()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    if rhs==2 then // y=interp1(y,xi)
        y=tree.rhs(1)
        xi=tree.rhs(2)
    elseif rhs>=3 then
        A=tree.rhs(3)
        if A.vtype==-1 then
            tree.lhs(1).dims==list(-1,-1)
        elseif A.vtype==1 then // y=interp1(x,y,xi,..)
            y=tree.rhs(2)
            xi=tree.rhs(3)
        elseif A.vtype==10 then // y=interp1(x,y,'method',..)
            y=tree.rhs(1)
            xi=tree.rhs(2)
        end
    end

    if  (rhs>=3 & (A.vtype==1 | A.vtype==10)) | rhs==2  then
        ydimknown=%t // ydimknown : a boolean, true if all dimensions of y are known
        xidimknown=%t // xidimknown : a boolean, true if all dimensions of xi are known
        for i=1:size(xi.dims)
            if xi.dims(i)==-1 then
                xidimknown=%f
                break
            end
        end
        for i=1:size(y.dims)
            if y.dims(i)==-1 then
                ydimknown=%f
                break
            end
        end
        if is_a_vector(y) then // y a vector
            if xidimknown  then // xi a scalar or a vector or a matrix or a hypermatrix
                tree.lhs(1).dims=xi.dims
            else // dimension(s) of xi is unknown
                tree.lhs(1).dims=list(-1,-1)
            end
        elseif ydimknown  then // y is a matrix or a hypermatrix
            if  xidimknown then // xi a scalar or a vector or a matrix or a hypermatrix
                tree.lhs(1).dims==xi.dims
                for i=1:size(xi.dims)-1
                    tree.lhs(1).dims(i+size(xi.dims))=y.dims(i+1)
                end
            else // dimension(s) of xi is unknown
                tree.lhs(1).dims==list(-1,-1)
            end
        else  // dimension(s) of y is unknown
            tree.lhs(1).dims=list(-1,-1)
        end
    end
    tree.lhs(1).type=Type(Double,Real)
endfunction