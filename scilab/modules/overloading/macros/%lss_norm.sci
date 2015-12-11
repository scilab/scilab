// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function y=%lss_norm(A,flag)
    if argn(2)==1 then flag=2,end
    if flag==2 then
        y=h2norm(A)
    elseif flag==%inf|flag=="inf" then
        y=h_norm(A)
    else
        error(msprintf(_("%s: Wrong value for input argument #%d: %d or %s expected.\n"),"%lss_norm",2,2,"inf"));
    end
endfunction
