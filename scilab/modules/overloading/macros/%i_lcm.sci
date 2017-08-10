// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Serge Steer
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [q,fact]=%i_lcm(p)
    //p=lcm(p) computes the lcm of polynomial vector p
    //[pp,fact]=lcm(p) computes besides the vector fact of factors
    //such that  p.*fact=pp*ones(p)
    //!

    k=find(p==0)
    if k<>[] then q=p(k(1)),fact=0*ones(p),fact(k)=1,return,end

    q=p(1);
    for k=2:size(p,"*")
        q=q/%i_gcd([q,p(k)])*p(k);
    end
    fact=q./p
endfunction
