function mtlb_echo(arg1,arg2)
// Copyright INRIA
// Emulation function for Matlab echo()
// V.C.

rhs=argn(2);

if rhs<=0 then
  mode(abs(mode()-1))
elseif rhs==1 then
  if arg1=="on" then
    mode(1);
  elseif arg1=="off" then
    mode(0);
  else
    error("Not yet implemented");
  end
else
  error("Not yet implemented");
end
endfunction
