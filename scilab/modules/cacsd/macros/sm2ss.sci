// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function des=sm2ss(sysmat)
// Generalized system-matrix to descriptor
// sysmat=[-s I + A , B; C , D]
// [n,n]=size(A) 

[LHS,RHS]=argn(0)
[nl,nc]=size(sysmat);
E=-coeff(sysmat,1);
if RHS==1 then
     k=nc;
     while E(:,k)==0*E(:,k)
     k=k-1;n=k;
     end
     l=nl;
     while E(l,:)==0*E(l,:)
     l=l-1;n1=l;
     end
end
if n1<>n then warning('sm2ss: cannot find n!');end
E=E(1:n,1:n);
if E<>eye(n,n) then error('sm2ss: not state-space system?');end
A=coeff(sysmat(1:n,1:n),0);
B=coeff(sysmat(1:n,n+1:nc),0);
C=coeff(sysmat(n+1:nl,1:n),0);
D=coeff(sysmat(n+1:nl,n+1:nc),0);
des=syslin([],A,B,C,D);
endfunction
