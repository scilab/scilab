function num=message(strings ,buttons)
//interface to message primitive to allow simple overloading for live demo 
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs==2 then
  num=x_message(strings ,buttons)
else
  num=1
  x_message(strings)
end
endfunction
