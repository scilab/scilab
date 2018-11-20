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

function r = %r_e(varargin)
    // %r_e(i,j,f) extraction f(i,j) in a rational matrix

    sel = 1:$-1
    if length(varargin)>2 & type(varargin(2))==1 & varargin(2)==0 
        sel = 1     // r(k,0)  k = linearized indices
    end
    f = varargin($);
    num = f.num
    den = f.den
    r = rlist(num(varargin(sel)), den(varargin(sel)), f.dt)
    if r.num==[] then r=[],end
endfunction
