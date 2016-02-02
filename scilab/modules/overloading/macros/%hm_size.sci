// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) INRIA - 2011 - Serge STEER
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

function varargout = %hm_size(M, job)

    // returns dimensions of an hyper matrix
    [lhs, rhs] = argn(0);
    dims = matrix(double(M.dims), 1, -1);
    if rhs == 2 then
        if type(job) == 10 then
            select job
            case "*" then
                varargout(1) = prod(dims);
            case "r" then
                varargout(1) = dims(1);
            case "c" then
                varargout(1) = dims(2);
            else
                error(msprintf(_("%s: Wrong value for input argument #%d: ''%s'', ''%s'' or ''%s'' expected.\n"), "size", 2, "r", "c", "*"))
            end
            return
        elseif type(job) == 1 then
            if (size(job,"*") > 1) | (job <= 0)  then
                error(msprintf(_("%s: Wrong value for input argument #%d: Scalar positive integer expected.\n"), "size", 2));
            end
            if job <= size(dims, "*") then
                varargout(1) = dims(job);
            else
                varargout(1) = 1;
            end
        else
            error(msprintf(_("%s: Wrong value for input argument #%d: Scalar positive integer or character string expected.\n"),"size", 2));
        end
        return
    end
    if lhs == 1 then
        varargout(1) = matrix(dims, 1, -1);
    else
        if lhs > size(dims, "*") then
            dims($+1:lhs) = 1;
        end
        for k = 1:lhs
            varargout(k) = dims(k);
        end
    end
endfunction
