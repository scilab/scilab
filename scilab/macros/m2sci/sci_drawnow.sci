function [stk,txt,top]=sci_drawnow()
// Copyright INRIA
set_infos('drawnow ignored',2)
txt=[txt;'// drawnow']
stk=list(' ','-2','0','0','1')
endfunction
