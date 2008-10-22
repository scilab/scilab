function x=signm(a)
//   signm - computes the matrix sign function.
//%CALLING SEQUENCE
//   x=signm(a)
//%PARAMETERS
//   a   : square hermitian matrix
//   x   : square hermitian matrix
//%DESCRIPTION
//   This macro is called by the function sign to compute square matrix
//   sign function.
//!
// Copyright INRIA
[m,n]=size(a)
if m<>n then error(20,1),end
flag=or(a<>a');
if flag then error('signm: non hermitian matrix'),end
[u,s]=schur(a)
x=u'*diag(sign(real(diag(s))))*u
endfunction
