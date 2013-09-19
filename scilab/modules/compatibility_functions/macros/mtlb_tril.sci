// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function y=mtlb_tril(x,k)
    // Emulation function for tril() Matlab function

    rhs=argn(2);
    if rhs==2 then
        if type(x)==10 then
            x=asciimat(x);
            tmp=tril(x,k);
            y=[]
            for l=1:size(tmp,1)
                tmp1=mstr2sci(ascii(tmp(l,:)));
                tmp1(find(tmp1==ascii(0)))=ascii(0)
                tmp1=strcat(tmp1);
                y=[y;tmp1]
            end
        elseif type(x)==4 then
            y=tril(bool2s(x),k)
        else
            y=tril(x,k);
        end
    else
        if type(x)==10 then
            x=asciimat(x);
            tmp=tril(x);
            y=[]
            for l=1:size(tmp,1)
                tmp1=mstr2sci(ascii(tmp(l,:)));
                tmp1(find(tmp1==ascii(0)))=ascii(0)
                tmp1=strcat(tmp1);
                y=[y;tmp1]
            end
        elseif type(x)==4 then
            y=tril(bool2s(x))
        else
            y=tril(x);
        end
    end
endfunction
