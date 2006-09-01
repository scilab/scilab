function r=%spb_g_spb(a,b)
//  perform logical elementwise or a|b where a and b are boolean sparse 
//  matrices
// Copyright INRIA
if prod(size(a))==1 then
  if full(a) then
    [mb,nb]==size(b)
    r=(ones(mb,nb)==1)
  else
    r=b
  end
elseif prod(size(b))==1  then
  if full(b) then
    [ma,na]==size(a)
    r=(ones(ma,na)==1)
  else
    r=a
  end
else
  r=a|b  
end
endfunction
