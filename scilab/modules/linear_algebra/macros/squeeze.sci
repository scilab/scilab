// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2007 - INRIA - Eric Dubois
// Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
// Copyright (C) 2011 - INRIA - Serge Steer (extension to Structs, Cells,
//                                           2D arrays of any types)
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function A = squeeze(A)

    // PURPOSE: Remove singleton dimensions, that is any dimension
    // for which the size of the input hypermatrix is 1; if the
    // input is a matrix, it is unaffected
    // ------------------------------------------------------------
    // INPUT:
    // * A = a hypermatrix or a matrix
    // ------------------------------------------------------------

    Dims=size(A);
    if or(size(Dims,"*") > 2 || typeof(A)==["ce","st"]) then
        Dims=size(A);
        newDims = Dims(Dims <> 1);
        if size(newDims,"*") < 2 then
            A=A(:);
        else
            A=matrix(A,newDims);
        end
    elseif type(A)<=10 then
        // it is a standard matrix nothing to do
    else
        error(msprintf(gettext("%s: Wrong type for input argument #%d.\n"),"squeeze",1));
    end

endfunction
