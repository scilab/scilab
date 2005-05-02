function [S,P,D,index]=projspec(A,tol)
//[S,P,D,index]=projspec(A)
//Spectral characteristics of A at 0
//S = reduced resolvent at 0 (S=-Drazin_inverse(A))
//P = spectral projection at 0
//D = Nilpotent operator at 0
//index = index of the 0 eigenvalue
//Author F.D.
//!
// Copyright INRIA
  [LHS,RHS]=argn(0)
  [n,n]=size(A);
  if RHS==1 then tol=1.d-6;end;
  // A=0 ?
  if norm(A,1) < %eps*n*n
    P=eye(A),D=A,S=0*P;index=1;
  end
  //     nonsingular A:  index 0
  if rcond(A) > tol
    S=inv(A),P=0*eye(A);D=P;index=0;return;
  end;
  //write(%io(2),'    rank A^k    rcond')
  //                    index 1
  index=1;
  [B,C,dim]=fullrf(A);
  if dim==0
    P=eye(n,n);S=0*P;D=A;return
  end;
  Ck=C;Bk=B;//write(%io(2),[dim,rcond(C*B)],'(7x,f3.0,6x,e9.3)');
  tst=rcond(Ck*Bk);
  if size(Ck,1)==1 then tst=norm(Ck*Bk,1);end
  if tst > tol then
    M=inv(C*B);P=eye(A)-B*M*C;S=B*M*M*C;D=0*A;return
  end
  //                  Higher index
  for k=2:n
    [B,C,dim]=fullrf(C*B);
    if dim==0
      P=eye(n,n);S=0*P;D=A;index=k;return;
    end;
    Bk=Bk*B;Ck=C*Ck;      // Bk*Ck = A^k  (Full rank factorization)
    index=k;
    //write(%io(2),[dim,rcond(C*B)],'(7x,f3.0,6x,e9.3)');
    if norm(C*B)*rcond(C*B) > tol,
      M=inv((C*B)**index);   //  M=inv(Ck*Bk);    (Alternative computation)
      P=eye(n,n)-Bk*M*Ck;   // P=eye(n,n)-Bk*inv(Ck*Bk)*Ck;  
      S=Bk*M*inv(C*B)*Ck;   // S=inv(A-P-D)+P   
      D=0.5*(A*P+P*A);return;
    end
  end;
  P=eye(n,n);S=0*P;D=A;index=k;return;
endfunction

