function [stk,txt,top]=sci_dir()
// Copyright INRIA
txt=[]
if rhs<0 then
  stk=list('unix_w(''ls'')','0','?','?','?')
else
  stk=list('unix_w(''ls ''+'+stk(top)(1)+')','0','?','?','?')
end
endfunction
