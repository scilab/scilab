function [stk,txt,top]=sci_fgetl()
// Copyright INRIA
v=lhsvarsnames();
if v==[] then 
  v=gettempvar(1)
  txt=v+'=mgetl('+stk(top)(1)+',1);if meof()~=0 then '+v+'=-1;end'
  stk=list(v,'0','1','1','10')
else
  txt=v+'=mgetl('+stk(top)(1)+',1);if meof()~=0 then '+v+'=-1;end'
  stk=list(v,'-2','1','1','10')
end
endfunction
