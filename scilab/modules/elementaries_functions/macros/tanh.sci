function [t]=tanh(z)
// 
//  PURPOSE 
//    element wise hyperbolic tangent
//
//  METHOD 
//    based on the formula  tanh(z) = -i tan(i z)
//
  if type(z)<>1 then error(53,1),end
  if isreal(z) then
     t = imag(tan(imult(z)))
  else
     t = -imult(tan(imult(z)))
  end
endfunction
