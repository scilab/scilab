// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function w=%msp_e(varargin)
    z=sci_sparse(varargin($));
    if length(varargin)==2 then
        w=mtlb_sparse(z(varargin(1)));
        return
    end
    i=varargin(1);j=varargin(2);
    w=mtlb_sparse(z(i,j));
endfunction
