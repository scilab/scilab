// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
function a=%cblock_f_cblock(a,b)
    if length(a)==1 then a=b,return,end
    if length(b)==1 then return,end

    if length(a)<>length(b) then error(6),end
    for k=2:length(a)
        setfield(k,[getfield(k,a);getfield(k,b)],a)
    end
endfunction
