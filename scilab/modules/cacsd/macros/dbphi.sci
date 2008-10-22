// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function [db,phi]=dbphi(repf,mod) 

[lhs,rhs]=argn(0)
if rhs==1 then mod='c',end
phi=phasemag(repf,mod);
db=20*log(abs(repf))/log(10);
endfunction
