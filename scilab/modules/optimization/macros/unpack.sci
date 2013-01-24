// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2011 - DIGITEO - Michael Baudin
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
function MM=unpack(M,blocksizes)
    //
    // Check input arguments
    [lhs,rhs]=argn();
    if rhs<>2 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"unpack",2));
    end
    //
    // Check type
    if typeof(M) <> "constant" then
        error(msprintf(gettext("%s: Wrong type for argument #%d: Real matrix expected.\n"),"unpack",1));
    end
    if typeof(blocksizes) <> "constant" then
        error(msprintf(gettext("%s: Wrong type for argument #%d: Real matrix expected.\n"),"unpack",2));
    end
    //
    // Check content
    if ~isreal(M) then
        error(msprintf(gettext("%s: Wrong type for argument #%d: Real matrix expected.\n"),"unpack",1));
    end
    if ~isreal(blocksizes) then
        error(msprintf(gettext("%s: Wrong type for argument #%d: Real matrix expected.\n"),"unpack",2));
    end
    if or(blocksizes<1) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Non-negative integers expected.\n"),"unpack",2));
    end
    if or(blocksizes<>floor(blocksizes)) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: An integer value expected.\n"),"unpack",2));
    end
    //
    // Proceed...
    MM=[]
    [mM,nM]=size(M)
    n=sum(blocksizes)
    for j=1:nM
        ptr=1
        Mu=[]
        for ni=blocksizes
            Mui=[]
            for l=1:ni
                Mui(l:ni,l)=M(ptr:ptr+ni-l,j)
                Mui(l,l:ni)=M(ptr:ptr+ni-l,j)'
                ptr=ptr+ni-l+1
            end
            Mu=[Mu;matrix(Mui,ni*ni,1)]
        end
        MM=[MM,Mu]
    end
endfunction
