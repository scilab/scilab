function [stk,txt,top]=sci_close()
// Copyright INRIA
if rhs<=0 then
  stk=list('xdel()','0','0','0','1')
elseif rhs==1 then
  if convstr(stk(top)(1))=='''all''' then
    txt='for %w=winsid(),xdel(%w);end'
    stk=list(' ','-2','0','0','1')
  elseif stk(top)(5)=='1' then
    stk=list('xdel('+stk(top)(1)+')','0','0','0','1')
  else
    set_infos(['Not enough information on '+stk(top)(1);
	       'using mtlb_close insted of xdel'],1)
    stk=list('mtlb_close('+stk(top)(1)+')','0','0','0','1')
  end
else
  txt='for %w=winsid(),xdel(%w);end'
  stk=list(' ','-2','0','0','1')
end
endfunction
