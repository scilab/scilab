function [t] = sinh(z)
// 
//  PURPOSE 
//     element wise hyperbolic sinus
//
//  METHOD 
//     based on the formula  sinh(z) = -i sin(i z)
//
  if type(z)<>1 then error(53,1),end
  if isreal(z) then 
     t = imag(sin(imult(z)))
  else
     t = -imult(sin(imult(z)))
  end
endfunction

