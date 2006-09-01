function r=%spb_h_spb(a,b)
//  perform logical elementwise and a&b where a and b are boolean sparse 
//  matrices
// Copyright INRIA
if prod(size(a))==1 then
  if full(a) then
    r=b
  else
    r=sparse([],[],size(b))<>0
  end
elseif prod(size(b))==1  then
  if full(b) then
    r=a
  else
    r=sparse([],[],size(a))<>0
  end
else
  r=a&b  
end
endfunction
