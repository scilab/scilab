function [t] = asinh(x)
  // 
  //  PURPOSE 
  //     element wise hyperbolic arcsinus
  //
  //  METHOD 
  //     based on the formula  asinh(z) = -i asin(i z)
  //
  if type(x)<>1 then error(53,1),end
  if isreal(x) then
    t = imag(asin(imult(x)))
  else
    t = -imult(asin(imult(x)))
  end 
endfunction
