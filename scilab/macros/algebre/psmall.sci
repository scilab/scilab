function [Q,M]=psmall(A,thres,flag)
// [Q,M]=psmall(A,thres,flag) 
//Projection on eigensubspace associated with eigenvalues
//with real part < thres (case flag='c') or with modulus <thres (flag='d')
//Projection is defined by Q*M. Eigenvalues of M*A*Q = eigenvalues
//of A with real part < thres (case flag='c',...).
//If [Q1,M1]== full rank factorization (fullrf) of eye-Q*M then evals of 
// M1*A*Q1 =evals of A with real part >= thres (case flag='d',...).
// See also pbig
//F.D.
//!
// Copyright INRIA
  [n,n]=size(A);
  thres=real(thres);
  if flag=='c' then 
    deff('[flag]=%smallei(x)','flag=real(x) < thres')
    deff('[flag]=%bigeig(x)','flag=real(x) >= thres')
    
  elseif flag=='d' then 
    deff('[flag]=%smallei(x)','flag=abs(x) < thres')
    deff('[flag]=%bigeig(x)','flag=abs(x) >= thres')
  else
    error('Invalid flag value, it must be '"c"' or '"d"' ')
  end
  // 
  [X,dsmall] = schur(A,%smallei);
  [Y,dbig] = schur(A,%bigeig);
  Q=X(:,1:dsmall);
  if Q==[] then M=[];return;end
  Y1=Y';
  M1=Y1(dbig+1:n,:);
  E=M1*Q;
  if rcond(E)>1.d-6 then
    M=E\M1;
  else
    //warning('bad conditionning--> balancing')
    [Ab,X0]=balanc(A);
    [X,dsmall] = schur(Ab,%smallei);X1=X*X0;Q=X1(:,1:dsmall);
    [Y,dbig] = schur(Ab,%bigeig);Y1=inv(X0)*Y';M=Y1(dbig+1:n,:);
    E=M*Q;
    M=E\M;
  end
endfunction
