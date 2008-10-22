// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function w=%msp_t(x)
if isempty(x) then
d=size(x);w=mtlb_sparse(d(2),d(1));
else
w=mtlb_sparse(sci_sparse(x)');
end;
endfunction
