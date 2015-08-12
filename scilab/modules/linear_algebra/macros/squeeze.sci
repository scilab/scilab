// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2007 - INRIA - Eric Dubois
// Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
// Copyright (C) 2011 - INRIA - Serge Steer (extension to Structs, Cells,
//                                           2D arrays of any types)
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function A = squeeze(A)

    // PURPOSE: Remove singleton dimensions, that is any dimension
    // for which the size of the input hypermatrix is 1; if the
    // input is a matrix, it is unaffected
    // ------------------------------------------------------------
    // INPUT:
    // * A = a hypermatrix or a matrix
    // ------------------------------------------------------------

    if or(typeof(A)==["hypermat","ce","st"]) then
        Dims=size(A)
        newDims = Dims(Dims <> 1) ;
        if size(newDims,"*") <2  then
            A=A(:)
        else
            A=matrix(A,newDims)
        end
    elseif type(A)<=10 then
        // it is a standard matrix nothing to do
    else
        error(msprintf(gettext("%s: Wrong type for input argument #%d.\n"),"squeeze",1))
    end

endfunction
