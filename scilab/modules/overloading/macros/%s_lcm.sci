// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Serge Steer
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [q,fact]=%s_lcm(p)
    //p=lcm(p) computes the lcm of polynomial vector p
    //[pp,fact]=lcm(p) computes besides the vector fact of factors
    //such that  p.*fact=pp*ones(p)
    //!

    k=find(p==0)
    if k<>[] then q=p(k(1)),fact=0*ones(p),fact(k)=1,return,end

    q=p(1);
    for k=2:size(p,"*")
        q=floor(q/%s_gcd([q,p(k)]))*p(k);
    end
    fact=floor(q./p)
endfunction
