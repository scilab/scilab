function [A,b]=aff2ab(lme,dimX,D,flag)
//Y,X,D are lists of matrices. 
//Y=lme(X,D)= affine fct of Xi's; 
//[A,b]=matrix representation of lme in canonical basis.
// if flag=='sp' A matrix is return in sparse storage.
// Copyright INRIA
  [LHS,RHS]=argn(0)
  if RHS==3 then flag='f';end

  nvars=0;
  for k=dimX'
    nvars=nvars+prod(k);
  end
  if part(flag,1)=='f' then
    x0=zeros(nvars,1);
    b=list2vec(lme(vec2list(x0,dimX),D));
    [p,un]=size(b);
    A=zeros(p,nvars);
    for k=1:nvars
      xi=x0;xi(k)=1;
      A(:,k)=list2vec(lme(vec2list(xi,dimX),D))-b;
      //   A=[A,sparse(list2vec(lme(vec2list(xi,dimX),D))-b)];
    end
  end

  if part(flag,1)=='s' then
    x0=zeros(nvars,1);
    b=list2vec(lme(vec2list(x0,dimX),D));
    A=[];
    for k=1:nvars
      xi=x0;xi(k)=1;
      A=[A,sparse(list2vec(lme(vec2list(xi,dimX),D))-b)];
    end
  end
endfunction
