function [stk,txt,top]=sci_gtext()
// Copyright INRIA
x=gettempvar(1)
y=gettempvar(2)
but=gettempvar(3)
txt='['+makeargs([but,x,y])+']=xclick()'
stk=list('xstring('+makeargs([x,y,stk(top)(1)])+')','0','0','0','1')
endfunction
