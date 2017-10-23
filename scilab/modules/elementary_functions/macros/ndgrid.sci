// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Bruno Pincon
// Copyright (C) 2015, 2017 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [varargout] = ndgrid(varargin)
    nbdim = length(varargin)
    if nbdim < 1 then
        msg = _("%s: Wrong number of input arguments: At least %d expected.\n")
        error(msprintf(msg, "ndgrid", 1))
    end
    if nbdim==1 then
        varargin(2) = varargin(1)
        nbdim = 2
    end

    dim = zeros(1,nbdim)
    for k = 1:nbdim
        if and(type(varargin(k)) ~= [1 2 4 8 10]) & typeof(varargin(k))~="rational"
            msg = _("%s: Wrong type for argument #%d: Booleans, Integers, Decimals, Complexes, Polynomials, Rationals or Texts expected.\n")
            error(msprintf(msg, "ndgrid", k))
        end
        dim(k) = size(varargin(k),"*")
        varargin(k) = matrix(varargin(k),1,-1) // force row form
    end

    varargout = list()
    // Manage input []
    for v = varargin
        if v==[]
            for i=1:nbdim
                varargout(i) = []
            end
            return
        end
    end

    // Processing:
    for k = 1:nbdim
        tmp = varargin(k)
        ind = ones(1,prod(dim(k+1:$))) .*. (1:size(tmp,"*")) ..
                .*. ones(1,prod(dim(1:k-1)))
        if typeof(tmp)~="rational"
            varargout(k) = matrix( tmp(ind) , dim )
        else
            varargout(k) = rlist(matrix(tmp(2)(ind), dim), matrix(tmp(3)(ind), dim), [])
        end
    end
endfunction
