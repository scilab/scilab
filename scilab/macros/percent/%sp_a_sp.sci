function s=%sp_a_sp(a,b)
// %spas - adds 2 sparse matrices (special cases)
//!
// Copyright INRIA
if size(a)==[1 1] then
  a=full(a)
  if a==0 then 
    s=b,
  else
    s=full(b)+a
  end
elseif size(b)==[1 1] then
  b=full(b)
  if b==0 then
    s=a
  else
    s=b+full(a)
  end
end
endfunction
