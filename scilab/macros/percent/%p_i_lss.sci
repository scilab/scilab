function s2=%p_i_lss(i,j,s1,s2)
//s2=%p_i_lss(i,j,s1,s2) <=> s2(i,j)=s1
//!
// Copyright INRIA
if type(i)==10 then  // sl('D')
  [lhs,rhs]=argn(0)
  if rhs<>3 then  error(21),end
  if i<>'D' then
    error('inserted element '+i+' has inconsistent type')
  end
  s2=s1;kf=5
  if size(s2(kf))<>size(j) then 
    warning('inserted element '+i+' has inconsistent dimension')
  end
  s2(kf)=j
  return
end

s1=tlist(['lss','A','B','C','D','X0','dt'],[],[],[],s1,[],[])
s2(i,j)=s1
endfunction
