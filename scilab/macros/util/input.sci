function [x]=input(message,flag)
// Copyright INRIA
  n=size(message,'*')
  for k=1:n-1
    mprintf(message(k)+'\n')
  end
  mprintf(message(n))
  x=mscanf("%s")
  if argn(2)==1 then
    x=evstr(x)
  end
endfunction
