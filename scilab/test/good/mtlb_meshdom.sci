// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [X,Y]=mtlb_meshdom(x,y)
[lhs,rhs]=argn()
x=matrix(x,1,-1);
y=matrix(y,-1,1);
X=ones(y)*x
Y=y($:-1:1)*ones(x)
endfunction
