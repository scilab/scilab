function [m,n]=%cblock_size(M)
  n=size(definedfields(M),'*')-1;
  m=size(getfield(2,M),1);
  n=0
  for k=2:size(definedfields(M),'*')
    n=n+size(getfield(k,M),2);
  end
  if argn(2)==1 then m=[m,n],end
endfunction
