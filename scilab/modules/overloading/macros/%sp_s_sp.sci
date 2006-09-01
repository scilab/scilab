function s=%sp_s_sp(a,b)
// %spas - adds 2 sparse matrices (special cases)
//!
// Copyright INRIA
if size(a)==[1 1] then
  a=full(a)
  if a==0 then 
    s=b,
  else
    s=a-full(b)
  end
elseif size(b)==[1 1] then
  b=full(b)
  if b==0 then
    s=a
  else
    s=full(a)-b
  end
end
endfunction
