function [stk,txt,top]=sci_choices()

// Copyright INRIA
RHS=[]
for k=1:rhs
  nam=stk(top-rhs+k)(1)
  RHS=[RHS nam]
end
num=gettempvar(1)
txt=['while %t;'
  num+'=x_choose(labels,header,''Close'')'
  'if '+num+'==0 then break,end'
  'execstr(callbacks('+num+'))'
  'end']
stk=list(' ','-2','0','0','1')
endfunction
