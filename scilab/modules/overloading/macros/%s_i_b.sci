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

function M=%s_i_b(varargin)

    rhs=argn(2)
    M=varargin($)
    if rhs<=4 then //type conversion
        M=bool2s(M)
        M(varargin(1:$-2))=varargin($-1)
    else //hypermatrix syntax
        if varargin($-1)<>[] then varargin($)=bool2s(M),end
        M=generic_i_hm(0,varargin(:))
    end
endfunction
