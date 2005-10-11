function [Bk,Ck]=fullrfk(A,k)
// This macro computes the full rank factorization of A^k i.e.
// Bk*Ck=A^k where Bk is full column rank and Ck full row rank.
// One has Range(Bk)=Range(A^k) and ker(Ck)=ker(A^k).
// For k=1 fullrfk is the same as fullrf.
// F.D (1990)
//!
// Copyright INRIA
  [lhs,rhs]=argn(0);
  if rhs==1,
    k=1;
  end
  [n,n]=size(A);
  //k=0
  if k==0,
    Bk=eye(n,n);Ck=Bk;
    return;
  end
  //k=1
  if k==1,
    [Bk,Ck]=fullrf(A);
    return;
  end
  [Bk,Ck]=fullrf(A);B=Bk;C=Ck;
  for l=2:k
    [B,C,dim]=fullrf(C*B);
    Bk=Bk*B;Ck=C*Ck;     // Bk*Ck = A^k  (Full rank factorization)
  end;
endfunction
