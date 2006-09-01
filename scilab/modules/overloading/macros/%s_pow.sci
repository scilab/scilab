function x=%s_pow(a,p)
//   g_pow - A^p special cases
//%CALLING SEQUENCE
//   X=g_pow(A,p)
//%PARAMETERS
//   A   : constant or square hermitian or diagonalizable matrix
//   X   : square matrix
//   p   : square matrix or scalar
//%DESCRIPTION
//This function is called by the operation ^ to compute A^p in special cases
// - A scalar and p square matrix
// - A square matrix p is not an integer
//!
// Copyright INRIA
[m,n]=size(a)
[mp,np]=size(p)
if m*n==1&mp==np then  //a^P
  flag=or(p<>p')
  r=and(imag(p)==0)&imag(a)==0
  if ~flag then 
    //Hermitian matrix
    [u,s]=schur(p);
    w=a.^diag(s);
    x=u*diag(a.^diag(s))*u';
    if r then
      x=real(x)
    end
  else
    [s,u,bs]=bdiag(p+0*%i);
    if maxi(bs)>1 then
      error('g_pow: unable to diagonalize!');
    end
    w=diag(s);
    x=u*diag(a.^diag(s))*inv(u);
  end
  if r then x=real(x), end
elseif m==n&mp*np==1 then  //A^p  p non integer
  flag=or(a<>a')
  if ~flag then 
    //Hermitian matrix
    r=and(imag(a)==0)
    [u,s]=schur(a);
    x=u*diag(diag(s).^p)*u';
    if r then
      if s>=0&imag(p)==0 then
	x=real(x)
      end
    end
  else
    //General matrix
    r=and(imag(a)==0)
    [s,u,bs]=bdiag(a+0*%i);
    if maxi(bs)>1 then
      error('g_pow: unable to diagonalize!');
    end
    x=u*diag(diag(s).^p)*inv(u);
  end
  if int(p)==p&r then x=real(x),end
else
  error(43)
end
endfunction
