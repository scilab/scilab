function[m,den]=nlev(a,z,rmax)
//[num,den]=nlev(a,z [,rmax])  calcule (z*eye-a)**(-1) par une version
//modifiee de l'algorithme de leverrier numeriquement plus stable.
//
//z     chaine de caracteres donnant le nom de la variable formelle
//rmax  parametre optionnel utilise pour bloc diagonaliser a (voir la
//      fonction bdiag)
//!
// origine f delebecque s. steer inria 1987
//
// Copyright INRIA
  [rhs,lhs]=argn(0);z=poly(0,z)
  if rhs==3 then [a,x,bs]=bdiag(a,rmax),
  else [a,x,bs]=bdiag(a),
  end;
  [m1 n1]=size(a)
  if m1<>n1 then error(20,1),end
  k=1;m=[];v=ones(1,n1);den=1;
  for n=bs';k1=k:k-1+n;
    //       algorithme de leverrier
    h=z*eye(n,n)-a(k1,k1)
    f=eye(n,n)
    for kl=1:n-1,
      b=h*f,
      d=-sum(diag(b))/kl,
      f=b+eye()*d,
    end;
    d=sum(diag(h*f))/n
    //
    den=den*d;
    l=[1:k-1,k+n:n1] ,
    if l<>[] then v(l)=v(l)*d;end
    m=[m,x(:,k1)*f];
    k=k+n;
  end;
  m=m*diag(v)*inv(x);
endfunction



