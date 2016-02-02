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

function s1=%p_i_r(varargin)
    //Author Serge Steer, INRIA
    //!
    s2=varargin($)
    s1=varargin($-1)
    //s2(i,...)=s1, s1 matrix of polynomials

    num=s2.num;
    sz2=size(num)
    ind=list(); for i=sz2,ind($+1)=1:i,end
    num(varargin(1:$-2))=s1
    den=ones(num);
    den(ind(:))=s2.den;
    den(varargin(1:$-2))=ones(s1);

    s1=rlist(num,den,s2.dt)
endfunction
