// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [dim,x]=unobs(A,C,tol)
// n first columns of x span the unobservable
// subspace of (A,C):
//          dim  
//          [*,*]
// X'*A*X = [0,*]
// 
//    C*X = [0,*]
// Copyright INRIA
[p,p]=size(A);tol=1.d-10*norm([A;C],1);
[n,w]=contr(A',C',tol);
x=[w(:,n+1:p),w(:,1:n)];
dim=p-n;
endfunction
