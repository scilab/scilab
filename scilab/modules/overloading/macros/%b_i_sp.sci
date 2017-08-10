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
function M=%b_i_sp(varargin)

    [lhs,rhs]=argn(0)
    M=varargin(rhs)
    N=bool2s(varargin(rhs-1))//inserted matrix
    if rhs<=4 then
        if rhs==3 then
            M(varargin(1))=N
        else
            M(varargin(1),varargin(2))=N
        end
    else
        error(msprintf(_("%s: Multidimensional sparse matrices are not handled.\n"),"%b_i_sp"));
    end

endfunction
