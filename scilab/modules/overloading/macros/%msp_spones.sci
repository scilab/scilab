// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [r] = %msp_spones(s)
// Replace nonzero elements with ones in matlab sparse matrix
// r=spones(s) generates a matrix with the sane sparsity structure as s,
// but with ones in the nonzero positions;
//
[ij,v,mn]=spget(s);
r=sparse(ij,ones(v),mn);
endfunction
