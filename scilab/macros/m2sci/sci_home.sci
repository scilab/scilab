function [stk,txt,top]=sci_home()
// Copyright INRIA
txt=[]
set_infos('home ignored',1)
txt=[txt; '// home']
stk=list(' ','-2','0','0','1')
endfunction
