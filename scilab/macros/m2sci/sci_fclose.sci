function [stk,txt,top]=sci_fclose()
// Copyright INRIA
txt=[]
if rhs==0 then
  stk=list('mclose()','0','1','1','1')
else
  if stk(top)(5)=='10' then
    stk=list('mclose(-2)','0','1','1','1')
  else
    stk=list('mclose('+stk(top)(1)+')','0','1','1','1')
  end
end
endfunction
