function r=isnan(x)
// Copyright INRIA
if x==[] then
  r=[]
else
  r=~(x==x)
end
endfunction
