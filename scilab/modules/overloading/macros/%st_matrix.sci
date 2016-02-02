// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - INRIA - Serge Steer
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function M=%st_matrix(M,varargin)
    //implements matrix function for structs
    if size(varargin)==1 then
        sz=varargin(1)
        if type(sz)<>1 then
            error(msprintf(_("%s: Wrong type for argument #%d: Real vector expected.\n"),"matrix",2))
        end
    else
        sz=[];
        for k=1:size(varargin)
            if size(varargin(k),"*")<>1|type(varargin(k))<>1 then
                error(msprintf(_("%s: Wrong type for argument #%d: Real scalar expected.\n"),"matrix",k+1))
            end
            sz=[sz varargin(k)]
        end
        sz=round(sz)
    end
    if size(sz,"*")<2 then sz=[sz,1],end
    Dims=size(M,"*")
    if Dims<>prod(sz) then
        error(msprintf(_("%s: input and output matrices  must have the same number of elements.\n"),"matrix"))
    end
    M.dims=int32(matrix(sz,1,-1))
endfunction
