function [x]=input(msg,flag)
// Copyright INRIA
  n=size(msg,'*')
  for k=1:n-1
    mprintf(msg(k)+'\n')
  end
  mprintf(msg(n))
  x=mscanf("%s")
  if argn(2)==1 then
    x=evstr(x)
  end
endfunction
