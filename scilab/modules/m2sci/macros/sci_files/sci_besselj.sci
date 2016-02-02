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

function  [tree]=sci_besselj(tree)
    // M2SCI function
    // Conversion function for Matlab besselj()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    if size(tree.lhs)==2 then // [y,ierr]=besselj(alpha,x)
        error(gettext("Bessel function is not implemented for 2 outputs arguments."))
    end

    if rhs==2 then
        [A,B]=getrhs(tree) // y=besselj(alpha,x)
    elseif rhs==3 then // y=besselj(alpha,x,1)
        [A,B,C]=getrhs(tree)
    end

    A=convert2double(A)
    B=convert2double(B)

    if is_real(B) then
        tree.lhs(1).type=Type(Double,Real)
    else
        tree.lhs(1).type=Type(Double,Unknown)
    end

    if is_empty(A) | is_empty(B) then // A or B is an empty
        tree.lhs(1).dims=list(0,0)
    elseif is_a_scalar(A) then // A is a scalar
        tree.lhs(1).dims=B.dims
    elseif is_a_scalar(B) then// B is a scalar
        tree.lhs(1).dims=A.dims
    else
        Aknowndim=%t // boolean false if A dimension is unknown
        Bknowndim=%t // boolean false if B dimension is unknown
        for i=1:size(A.dims)
            if A.dims(i)==-1 then
                Aknowndim=%f
                break
            end
        end
        for i=1:size(B.dims)
            if B.dims(i)==-1 then
                Bknowndim=%f
                break
            end
        end
        if is_a_vector(A) then // A is a vector
            if is_a_vector(B) then // B is a vector
                if A.dims==B.dims then  // both A and B are column vectors or both A and B are line vectors
                    tree.lhs(1).dims=B.dims
                else // A is a line vector and B is a column vector, or A is a column vector and B is a line vector
                    tree.lhs(1).dims(1)=B.dims(1)*B.dims(2)
                    tree.lhs(1).dims(2)=A.dims(1)*A.dims(2)
                end
            end
        elseif ~(Aknowndim & Bknowndim)  then // A or B dimension(s) is unknown
            tree.lhs(1).dims==list(-1,-1)
        elseif A.dims==B.dims then
            tree.lhs(1).dims=A.dims
        else
            tree.lhs(1).dims==list(-1,-1)
        end
    end
endfunction
