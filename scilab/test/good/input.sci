function [x]=input(msg,flag)
// Copyright INRIA
  fmt='%['+ascii(32)+'-'+ascii(255)+']'// a tricky way to get all ascii codes  sequences
  n=size(msg,'*')
  for k=1:n-1
    mprintf(msg(k)+'\n')
  end
  if argn(2)==2 then
    mprintf(msg(n))
     x=mscanf(fmt) 
  else
    while %t
      mprintf(msg(n))
      x=stripblanks(mscanf(fmt))
      if length(x)==0 then x='[]',end
      ierr=execstr('x='+x,'errcatch')
      if ierr==0 then break,end
      mprintf(strcat(lasterror(),'\n'))
    end
  end
endfunction
