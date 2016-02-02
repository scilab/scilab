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

function [Y]=mtlb_beta(A,B)
    // Emulation function for Matlab beta()

    if (size(A,"*")==0 & size(B,"*")==0) | (size(A,"*")==0 & type(B)==1) | (size(B,"*")==0 & type(A)==1) then
        Y=[]
    elseif size(A,"*")==1 & size(B,"*")<>1 then
        tmp=A
        A=B
        A(:)=tmp
        Y=beta(A,B)
    elseif size(A,"*")<>1 & size(B,"*")==1 then
        tmp=B
        B=A
        B(:)=tmp
        Y=beta(A,B)
    else
        Y=beta(A,B)
    end

endfunction


