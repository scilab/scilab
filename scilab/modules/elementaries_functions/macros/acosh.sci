function [t] = acosh(z)
  // 
  //  PURPOSE 
  //     element wise hyperbolic arccosinus
  //
  //  METHOD 
  //     based on the formula : 
  //
  //     acosh(z) = sign(-imag(acos(z)) i acos(z)
  //
  //     but as in Scilab sign(0) = 0 (and this don't
  //     work in the formula which needs sign(0)=1)
  //     the following slower trick is used :
  //
  //     sign(-x) = 2*(0.5 - bool2s(x>0))
  //
  if type(z)<>1 then error(53,1),end
  if isreal(z) then
    if min(z) < 1 then
      // result is complex
      u = acos(z)
      t = 2*(0.5 - bool2s(imag(u)>0)).*imult(u)
    else
      // result is real
      t = imag(acos(z))
    end
  else
    u = acos(z)
    t = 2*(0.5 - bool2s(imag(u)>0)).*imult(u)
  end
endfunction
