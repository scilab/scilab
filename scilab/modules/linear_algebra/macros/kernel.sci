function kerA=kernel(A,tol,flag)
//Syntax : [x]=kernel(A [,tol [,flag]])
// flag = 'svd' or 'sqr' no flag if A sparse 
// Copyright INRIA
  if type(A)==1 then
    [lhs,rhs]=argn(0);
    [n,m]=size(A);
    select rhs
    case 1 then
      [kerA,rk]=colcomp(A);
    case 2 then
      [kerA,rk]=colcomp(A,'svd',tol)
    case 3 then
      [kerA,rk]=colcomp(A,flag,tol)
    end
    kerA=kerA(:,1:m-rk);return;
  end
  if type(A)==5 then
    [lhs,rhs]=argn(0);
    [ma,na]=size(A);
    if rhs==3 then error('SVD and QR not implemented in sparse');end
    if rhs==2 then tol=1.d-10;else tol=%eps;end
    %tol=tol*maxi(abs(A))*max(ma,na);
    if ma<na then 
      A=[A;sparse([],[],[na-ma,na])];
    end
    if ma>na then
      A=A'*A;ma=na;
    end
    [ptr,rkA]=lufact(A,[%tol,0.001]);
    [P,L,U,Q]=luget(ptr);
    ludel(ptr);
    U=clean(U);
    [ptrU,rk]=lufact(U,[%tol,.001]);
    nma=max(na,ma);
    Y=[sparse([],[],[rkA,nma-rkA]);speye(nma-rkA,nma-rkA)];
    kerA=[];
    for k=1:na-rkA
      bb=full(Y(:,k));
      ww=sparse(lusolve(ptrU,bb));
      kerA=[kerA,ww];
    end     
    ludel(ptrU);
    if na<>rkA then 
      kerA=clean(Q'*kerA);
    end
    if ma>na then kerA=kerA(1:na,:);
    end
  else
    error('Not implemented')
  end
endfunction


