// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [Z,U]=zeropen(Sl)
//Z = sE - F = zero pencil of Sl=[A,B,C,D]
// With U row compression of [B;D] i.e, U*[B;D]=[0;*]; 
//U*[-sI+A |B;   [ Z |0;      
//      C  |D] = [ * |*]     

s=poly(0,'s');
[A,B,C,D]=abcd(Sl);
BD=[B;D];
[n,m]=size(BD);
[U,rk]=rowcomp(BD);U=[U(rk+1:n,:);U(1:rk,:)];
W=U*([-s*eye()+A;C])
Z=W(1:n-rk,:);
endfunction
