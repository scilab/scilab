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

function M=%s_i_hm(varargin)

    //insertion of a matrix of float in an hypermatrix
    M=varargin($)
    select type(M.entries)
    case 1 then //destination is an hypermat of double
        M=generic_i_hm(0,varargin(:))
    case 2 then //destination is an hypermat of polynomials
        M=generic_i_hm(0,varargin(:))
    case 4 then //destination is an hypermat of boolean
        N=varargin($-1)
        if N<>[] then N=N<>0,end
        M=generic_i_hm(%f,varargin(1:$-2),N,M)
    case 8 then //destination is an hypermat of integer
        it=inttype(M.entries)
        z=iconvert(0,it)
        N=varargin($-1)
        if N<>[] then N=iconvert(N,it),end
        M=generic_i_hm(z,varargin(1:$-2),N,M)
    else
        M=generic_i_hm(0,varargin(:))
    end
endfunction
