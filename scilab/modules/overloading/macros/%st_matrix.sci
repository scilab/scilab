// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - INRIA - Serge Steer
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
