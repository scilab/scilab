function [f,e]=log2(x)
if argn(1)==1 then
  f=log(x)/log(2)
else
  [f,e]=frexp(x)
end
endfunction
