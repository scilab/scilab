function [z]=foo(x,y)
// Copyright INRIA
[in,out]=argn(0);
if x=0 then,
     error('division by zero');
end,
slope=y/x;
pause,
z=sqrt(slope);
s=resume(slope);
