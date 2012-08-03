// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Michael Baudin
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function s=%sp_string(A)
    [ij,v,mn]=spget(A);
    s = msprintf("(%d,%d) sparse matrix",mn(1),mn(2))
    s = [s; "("+string(ij(:,1)) + "," + string(ij(:,2))+")    " + string(v)]
endfunction	

