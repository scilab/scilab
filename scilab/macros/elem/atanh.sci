function [z] = atanh(z)
  // 
  //  PURPOSE 
  //     element wise hyperbolic arctangent
  //
  //  METHOD 
  //     based on the formula  atanh(z) = i atan(-i z)
  //
  if type(z)<>1 then error(53),end
  if isreal(z) then 
    k=find(abs(z)>1)
    z(k) = imult(atan(-imult(z(k))))// result is complex 
    k=find(abs(z)<=1)
    z(k)= -imag(atan(-imult(z(k))))// result is real
  else
    z = imult(atan(-imult(z)))
  end
