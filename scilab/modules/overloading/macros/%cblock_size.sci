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
function [m,n]=%cblock_size(M)
    n=size(definedfields(M),"*")-1;
    m=size(getfield(2,M),1);
    n=0
    for k=2:size(definedfields(M),"*")
        n=n+size(getfield(k,M),2);
    end
    if argn(2)==1 then m=[m,n],end
endfunction
