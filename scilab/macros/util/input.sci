function [x]=input(message,flag)
// Copyright INRIA
[LHS,RHS]=argn(0)
n=size(message,'*')
for k=1:n-1
  mprintf(message(k)+'\n')
end
mprintf(message(n))
if RHS==1 then
x=evstr(read(%io(1),1,1,'(a)'))
end
if RHS==2 then
x=read(%io(1),1,1,'(a)')
end
