function r=mtlb_exist(nam,wh)
// Copyright INRIA
// Emulation function for exist() Matlab function
// V.C.

[lhs,rhs]=argn(0)
fptr=funptr(nam)
r=0
if rhs==1 then
  if fptr<>0 then 
    fptr=int(fptr/100)
    if fptr<=500 then
      r=5
    else
      r=3
    end
  elseif exists(nam)==1 then
    if type(nam)==11|type(nam)==13 then
      r=2
    else
      r=1
    end
  end
else
  select wh
  case "var" then
    if exists(nam)==1 then r=1,end
  case "builtin" then
    if fptr<>0 then 
      fptr=int(fptr/100)
      if fptr<=500 then
	r=5
      else
	r=3
      end
    end
  case "file" then
  case "dir" then
  end
end
endfunction
