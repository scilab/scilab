function [Ws,Fs1]=rowshuff(Fs,alfa)
// Shuffle algorithm: Given the pencil Fs=s*E-A, returns Ws=W(s) 
// (square polynomial matrix) such that:
// Fs1 = s*E1-A1 = W(s)*(s*E-A) is a pencil with non singular E1 matrix.
// This is possible iff the pencil Fs = s*E-A is regular (i.e. invertible).
// The poles @ infinity of Fs are put to alfa and the zeros of Ws are @ alfa.
// Note that (s*E-A)^-1 = (s*E1-A1)^-1 * W(s) = (W(s)*(s*E-A))^-1 *W(s)
// F.D.
//!
// Copyright INRIA
  [LHS,RHS]=argn(0);
  if RHS==1 then
    alfa=0;
  end
  [E,A]=pen2ea(Fs);
  //     E is non singular: --> exit
  if rcond(E) >= 1.d-5 then W=eye(E);Fs1=Fs;return;end
  //     E is singular:
  s=poly(0,'s');tol=1.d-10*(norm(E,1)+norm(A,1));
  [n,n]=size(E);Ws=eye(n,n);
  //     
  rk=0;i=0;
  while rk  < n
    if i==n then error('rowshuffle: singular pencil!');W=[];end
    [W,rk]=rowcomp(E);
    if rk==n then return;end
    W1=W(1:rk,:);W2=W(rk+1:n,:);
    E=[W1*E;
       -W2*A];
    A=[W1*A;
       -alfa*W2*A];
    Fs1=s*E-A;
    //     Update 
    Ws=[eye(rk,rk),zeros(rk,n-rk);
        zeros(n-rk,rk),(s-alfa)*eye(n-rk,n-rk)]*W*Ws;
    i=i+1;
  end
endfunction

