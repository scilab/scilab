// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Serge STEER
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function I = sub2ind(dims, varargin)
    //sub2ind is used to determine the equivalent single index
    //corresponding to a given set of subscript values.

    //I = sub2ind(dims,i1,i2,..) returns the linear index equivalent to the
    //row,  column, ... subscripts in the arrays i1,i2,..  for an matrix of
    //size dims.

    //I = sub2ind(dims,Mi) returns the linear index
    //equivalent to the n subscripts in the columns of the matrix Mi for a matrix
    //of size dims.

    [lhs, rhs] = argn();
    if rhs < 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "sub2ind", 1));
    end

    d = [1;cumprod(matrix(dims(1:$-1),-1,1))]
    for i=1:size(varargin)
        if varargin(i)==[] then I=[],return,end
    end

    if size(varargin)==1 then //subindices are the columns of the argument
        I=(varargin(1)-1)*d+1
    else //subindices are given as separated arguments
        I=1
        for i=1:size(varargin)
            I=I+(varargin(i)-1)*d(i)
        end
    end
endfunction
