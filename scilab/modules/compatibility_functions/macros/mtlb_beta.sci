// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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


