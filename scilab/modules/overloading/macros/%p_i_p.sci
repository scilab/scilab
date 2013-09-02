// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - serge Steer
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function M=%p_i_p(varargin)
    //insertion of an polynomial matrix in an matrix of polynomials for more than 2 indices

    M=varargin($)
    M=mlist(["hm","dims","entries"],int32(size(M)),M(:))
    varargin($)=M;
    M=generic_i_hm(0,varargin(:))
endfunction
