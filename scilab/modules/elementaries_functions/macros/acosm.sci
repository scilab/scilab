function t=acosm(x)
// Matrix wise Arccosine Cosine-inverse
//!
// Copyright INRIA
  if type(x)<>1 then error(53,1),end
  [m,n]=size(x)
  if m<>n then 
    error(20)
  else 
    t=-%i*logm(x+%i*sqrtm(eye()-x*x))
  end
endfunction
