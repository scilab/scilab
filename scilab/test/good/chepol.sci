function Tn=chepol(n,var)
//Chebychev polynomial
//  n   :Polynomial order
//  var :Polynomial variable (character string)
//  Tn  :Polynomial in var
//
//!
//Author F.D.
// Copyright INRIA
if n==0 then
  Tn=poly(1,var,'coeff'),
elseif n==1 then
  Tn=poly(0,var);
else
  T0=poly(1,var,'coeff');
  T1=poly(0,var)
  for k=2:n
    Tn=2*poly(0,var)*T1-T0
    [T1,T0]=(Tn,T1);
  end
end
    
endfunction
