function x=mtlb_double(y)
// Copyright INRIA
if type(y)==10 then 
  x=asciimat(y)
elseif type(y)==4 then 
  x=bool2s(y)
elseif type(y)==8 then
  x=double(y)
else
  x=y
end
endfunction


