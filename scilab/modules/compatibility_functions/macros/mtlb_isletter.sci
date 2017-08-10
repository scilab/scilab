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

function tf=mtlb_isletter(A)
    // Translation function for isletter() Matlab function

    if type(A)==10 then
        tf = (asciimat(A)>=65&asciimat(A)<=90)|(asciimat(A)>=97&asciimat(A)<=122)
    else
        tf = zeros(A)
    end
endfunction
