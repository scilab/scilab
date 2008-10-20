function x=cosm(a)
//   cosm - computes the matrix cosine 
//%CALLING SEQUENCE
//   x=cosm(a)
//%PARAMETERS
//   a   : square hermitian or diagonalizable matrix
//   x   : square hermitian matrix
//!
// Copyright INRIA
  if type(a)<>1 then error(53,1),end
  if a==[] then x=[],return,end

  if norm(imag(a),1)==0 then
    x=real(expm(%i*a))
  else
    x=0.5*(expm(%i*a)+expm(-%i*a));
  end
endfunction
