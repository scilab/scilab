// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 1995-2010 - INRIA - Serge Steer
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function d=%r_tril(a,k)
    if argn(2)<2 then
        k=0,
    else
        if type(k)<>1 then error(53,2);end
        if size(k,"*")<>1 then error(89,2);end
        if ~isreal(k) then error(52,2);end
    end

    d=rlist(tril(a.num,k),tril(a.den,k)+triu(ones(a.den),k+1),a.dt)
endfunction
