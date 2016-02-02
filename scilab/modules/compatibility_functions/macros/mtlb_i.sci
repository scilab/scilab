// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function a=mtlb_i(a,i,b)
    if size(a,1)==1 then
        a(i)=matrix(b,1,size(b,"*"))
    elseif size(a,2)==1 then
        a(i)=matrix(b,size(b,"*"),1)
    else
        a(i)=b
    end
endfunction
