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

function y=mtlb_triu(x,k)
    // Emulation function for triu() Matlab function

    rhs=argn(2);
    if rhs==2 then
        if type(x)==10 then
            x=asciimat(x);
            tmp=triu(x,k);
            y=[]
            for l=1:size(tmp,1)
                tmp1=mstr2sci(ascii(tmp(l,:)));
                tmp1(find(tmp1==ascii(0)))=ascii(0)
                tmp1=strcat(tmp1);
                y=[y;tmp1]
            end
        elseif type(x)==4 then
            y=triu(bool2s(x),k)
        else
            y=triu(x,k);
        end
    else
        if type(x)==10 then
            x=asciimat(x);
            tmp=triu(x);
            y=[]
            for l=1:size(tmp,1)
                tmp1=mstr2sci(ascii(tmp(l,:)));
                tmp1(find(tmp1==ascii(0)))=ascii(0)
                tmp1=strcat(tmp1);
                y=[y;tmp1]
            end
        elseif type(x)==4 then
            y=triu(bool2s(x))
        else
            y=triu(x);
        end
    end
endfunction
