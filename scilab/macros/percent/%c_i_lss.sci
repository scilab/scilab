function s2=%c_i_lss(i,j,s1,s2)
// Copyright INRIA
if type(i)==10 then  // sl('dt')
  [lhs,rhs]=argn(0)
  if rhs<>3 then  error(21),end
  if i<>'dt' then
    error('inserted element '+i+' has inconsistent type')
  end
  s2=s1;kf=7
  if j<>'c'&j<>'d' then 
    error('inserted element '+i+' must be ''c'' or ''d'' or a scalar')
  end
  s2(kf)=j
  return
end
endfunction
