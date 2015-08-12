// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Serge Steer
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
function r=%b_iconvert(val,it)

    [m,n]=size(val)
    if m*n==0 then r=[],return,end
    k=find(val)
    r(m,n)=iconvert(0,it)
    r(find(val))=iconvert(1,it)
endfunction
