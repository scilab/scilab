// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function kdf=tokenpos(str,sep)
    //return a vector of indices giving the beginning of the included
    //tokens
    if argn(2)<2 then sep=[" ",ascii(9)],end

    if size(str,"*")<> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A character string expected.\n"),"tokenpos",1));
    end
    if or(length(sep)<>1) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A character or vector of characters expected.\n"),"tokenpos",2));
    end
    str=sep(1)+str+sep(1)
    k1=strindex(str,sep)
    i=find(k1(2:$)-k1(1:$-1)>1)
    if i<>[] then
        kdf=[k1(i);k1(i+1)-2]'
    else
        kdf=[]
    end
endfunction
