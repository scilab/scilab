// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function d=%c_tril(a,k)

    [lhs,rhs]=argn(0)
    if rhs==1 then k=0,end

    [m,n]=size(a)
    i=find(tril(ones(a),k))
    a=matrix(a,m*n,1)
    d=emptystr(m*n,1)
    d(i)=a(i)
    d=matrix(d,m,n)
endfunction
