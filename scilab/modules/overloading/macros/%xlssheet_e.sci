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

function R=%xlssheet_e(varargin)
    //extract a subsheet out of an xls sheet
    s=varargin($)
    R=s.value;T=s.text
    R=R(varargin(1:$-1))
    T=T(varargin(1:$-1))
    if and(isnan(R)) then
        R=T
    elseif or(isnan(R)) then
        R=mlist(["xlssheet","name","text","value"],"",T,R)
    end
endfunction
