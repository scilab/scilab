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
deff('[flag]=%csmall(x)',[...
   'ls=x(1);';
   'select ls;';
   'case 1 then flag= x(2) < thres*x(3);';
   'case 2 then flag= x(2) < thres/2;';
   'end;';
   'if flag then flag=1;else flag=-1;end']);

deff('[flag]=%dsmall(x)',[...
   'ls=x(1);';
   'select ls;';
   'case 1 then flag =(abs(x(2)) < thres*abs(x(3)));';
   'case 2 then flag =(abs(x(3)) < thres*thres);';
   'end;';
   'if flag then flag=1;else flag=-1;end']);

deff('[flag]=%cbigeig(x)',[...
     'ls=x(1);';
     'select ls;';
     'case 1 then flag= x(2) >= thres*x(3);';
     'case 2 then flag= x(2) >= thres/2 ;';
     'end;';
     'if flag then flag=1;else flag=-1;end']);

deff('[flag]=%dbigeig(x)',[...
     'ls=x(1);';
     'select ls;';
     'case 1 then flag= (abs(x(2)) >= thres*abs(x(3)));';
     'case 2 then flag= (abs(x(3)) >= thres*thres) ;';
     'end;';
     'if flag then flag=1;else flag=-1;end']);

 [n,n]=size(A);
 thres=real(thres);
if flag=='c' then %smallei=%csmall;%bigeig=%cbigeig;end
if flag=='d' then %smallei=%dsmall;%bigeig=%dbigeig;end
// 
 [X,dsmall] = schur(A,%smallei);
 [Y,dbig] = schur(A,%bigeig);
 Q=X(:,1:dsmall);
 if Q==[] then M=[];return;end
 Y1=Y';
 M1=Y1(dbig+1:n,:);
 E=M1*Q;
if rcond(E)>1.d-6 then
     M=E\M1;return;
                  else
//warning('bad conditionning--> balancing')
     [Ab,X0]=balanc(A);
     [X,dsmall] = schur(Ab,%smallei);X1=X*X0;Q=X1(:,1:dsmall);
     [Y,dbig] = schur(Ab,%bigeig);Y1=inv(X0)*Y';M=Y1(dbig+1:n,:);
     E=M*Q;
     M=E\M;
end
