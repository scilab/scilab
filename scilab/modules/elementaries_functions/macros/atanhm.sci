function t=atanhm(x)
//Matrix wise Hyperbolic tangent inverse 

// Copyright INRIA
  if type(x)<>1 then error(53,1),end
  if x==[] then t=[],end
  [m,n]=size(x)
  if m<>n then 
    error(20)
  else 
    t=logm((eye()+x)*sqrtm(eye(x)/(eye()-x*x)))
  end
endfunction
