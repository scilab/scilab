function r=%s_h_s(a,b)
//computes a&b for a and b matrices of scalar
// Copyright INRIA
if a==[]|b==[] then 
  r=[]
else
  r=(a<>0)&(b<>0)
end
endfunction
