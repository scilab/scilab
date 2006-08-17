function [Z,U]=zeropen(Sl)
//Z = sE - F = zero pencil of Sl=[A,B,C,D]
// With U row compression of [B;D] i.e, U*[B;D]=[0;*]; 
//U*[-sI+A |B;   [ Z |0;      
//      C  |D] = [ * |*]     
// Copyright INRIA
s=poly(0,'s');
[A,B,C,D]=abcd(Sl);
BD=[B;D];
[n,m]=size(BD);
[U,rk]=rowcomp(BD);U=[U(rk+1:n,:);U(1:rk,:)];
W=U*([-s*eye()+A;C])
Z=W(1:n-rk,:);
endfunction
