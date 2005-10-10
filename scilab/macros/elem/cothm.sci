function x=cothm(a)
// hyperbolic co-tangent of square matrix 
// Copyright INRIA
  if type(a)<>1 then error(53,1),end
  if a==[] then x=[],return,end
  [m,n]=size(a)
  if m<>n then error(20),end
  //diagonalization
  [x,t,bs]=bdiag(a+0*%i*ones(a),1/%eps)
  if find(bs>1)<>[] then error('Matrix is not diagonalisable'),end
  x=t*diag(coth(diag(x)))/t
  if and(imag(a)==0) then x=real(x),end
endfunction
