// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
