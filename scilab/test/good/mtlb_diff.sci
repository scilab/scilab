function y=mtlb_diff(x,N,dim)
// Copyright INRIA
// Emulation function for Matlab diff()
// V.C.

[lhs,rhs]=argn()
if rhs==1 then N=1,end
if rhs<=2 then
  if length(size(x))>2 then
    dim=firstnonsingleton(x)
  elseif size(x,1)==1 | size(x,2)==1 then
    dim=0
  else
    dim='r'
  end
    
end

if dim==0 then
  y=diff(x,N)
else
  if N>=size(x,"*") then
    y=[]
    return
  else
    for k=1:N
      x=diff(x,1,dim)
      if size(x,dim)==1 then
	dim=firstnonsingleton(x)
      end
    end
  end
  if x==[] then
    y=0
  else
    y=x
  end
end
endfunction
