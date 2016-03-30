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

function s1=%r_i_r(varargin)
    //author Serge Steer INRIA
    s1=varargin($-1)
    s2=varargin($)
    // %r_i_r(i,j,s1,s2) insertion  s2(i,j)=s1

    [s1,s2]=sysconv(s1,s2)
    n=s2.num;
    n(varargin(1:$-2))=s1.num,
    if ndims(s2.den)<=2 then
        d=ones(n);
        [n1,n2]=size(s2.den)
        d(1:n1,1:n2)=s2.den;
        d(varargin(1:$-2))=s1.den
    else
        d=generic_i_hm(1,varargin(1:$-2),s1.den,s2.den)
    end
    s1=rlist(n,d,s1.dt)
endfunction
