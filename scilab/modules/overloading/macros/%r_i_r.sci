// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Serge STEER
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2018 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function s1 = %r_i_r(varargin)
    s2 = varargin($)
    s1 = varargin($-1)
    // %r_i_r(i,j,s1,s2) insertion  s2(i,j,..) = s1

    sel = 1:$-2
    if type(varargin(2))==1 & varargin(2)==0 then
        sel = 1     // linearized indices
    end
    num = s2.num;
    num(varargin(sel)) = s1.num
    den = s2.den
    den(varargin(sel)) = s1.den
    den(num==0) = 1;
    s1 = rlist(num, den, s2.dt)
endfunction
