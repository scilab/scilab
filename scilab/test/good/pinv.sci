function X=pinv(A,tol)
// Copyright INRIA
//Pseudo inverse
if type(A)==1 then
  if A==[] then X=[],return,end
  
  [U,S,V] = svd(A,"e");
  S = diag(S)
  if argn(2) < 2
    tol = max(size(A)) * S(1) * %eps;
  end
  r=size(find(S>tol),'*') //Rank
  if r == 0 then
    X = zeros(A');
  else
    //X=V1*inv(S1)*U1'
    X = V(:,1:r)* diag(ones(r,1)./S(1:r)) *U(:,1:r)';
  end
else
  [t,n]=typename();n=stripblanks(n(find(t==type(A))))
  fun='%'+n+'_pinv'
  if exists(fun)==1 then
    if argn(2)==1 then
      execstr('X='+fun+'(A)')
    else
      execstr('X='+fun+'(A,tol)')
    end
  else
    error('pinv not defined for type ""'+n+'"" .'+..
	  'Check argument or define function '+fun)
  end
end
endfunction
    
