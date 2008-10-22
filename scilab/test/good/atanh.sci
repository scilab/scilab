function [t] = atanh(z)
  // 
  //  PURPOSE 
  //     element wise hyperbolic arctangent
  //
  //  METHOD 
  //     based on the formula  atanh(z) = i atan(-i z)
  //
  if type(z)<>1 then error(53,1),end
  if isreal(z) then 
    if max(abs(z)) > 1 then  // il faut faire le test ds ce sens pour
                             // les Nan (sinon on obtient NaN + i NaN ! )
      // result is complex 
      t = imult(atan(-imult(z)))
    else
      // result is real
      t= -imag(atan(-imult(z)))
    end
  else
    t = imult(atan(-imult(z)))
  end
endfunction

