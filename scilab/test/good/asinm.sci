function t=asinm(x)
//Matrix wise Sine-inverse of x
// Copyright INRIA
  if type(x)<>1 then error(53,1),end
  [m,n]=size(x)
  if m<>n then 
    error(20)
  else 
    t=-%i*logm(%i*x+sqrtm(eye()-x*x))
  end
endfunction
