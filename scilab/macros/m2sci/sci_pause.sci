function [stk,txt,top]=sci_pause()
// Copyright INRIA
txt=[]
if rhs<1 then
  stk=list('halt()','0','0','0','1')
else
  if stk(top)(5)=='10' then
    set_infos('Warning: pause('+stk(top)(1)+') ignored',1)
    stk=list(' ',-2,'0','0','1')
  else
    stk=list('xpause(1000*('+stk(top)(1)+'))','0','0','0','1')
  end
end
endfunction
