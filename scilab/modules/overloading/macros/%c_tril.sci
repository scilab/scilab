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
