function mtlb_grid(arg1,arg2)
// Copyright INRIA
// Emulation function for Matlab grid
// V.C.

rhs=argn(2)
if rhs<=0 then // toggle
  a=gca();
  if and(a.grid==[-1 -1]) then
    a.grid=[1 1]
  else
    a.grid=[-1 -1]
  end
elseif rhs==1 then
  if type(arg1)==9 then // toggle
    if and(arg1.grid==[-1 -1]) then
      arg1.grid=[1 1]
    else
      arg1.grid=[-1 -1]
    end
  else
    a=gca();
    if arg1=="on" then
      a.grid=[1 1]
    elseif arg1=="off" then
      a.grid=[-1 -1]
    elseif arg1=="minor" then
      warning("No minor grid");
      if and(a.grid==[-1 -1]) then
	a.grid=[1 1]
      else
	a.grid=[-1 -1]
      end
    else
      error("Wrong option");
    end
  end
elseif rhs==2 then
  if arg2=="on" then
    arg1.grid=[1 1]
  elseif arg2=="off" then
    arg1.grid=[-1 -1]
  elseif arg2=="minor" then
    warning("No minor grid");
    if and(arg1.grid==[-1 -1]) then
      arg1.grid=[1 1]
    else
      arg1.grid=[-1 -1]
    end
  else
    error("Wrong option");
  end
end
endfunction
