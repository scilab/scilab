function [stk,txt,top]=sci_unix()
// Copyright INRIA
txt=[]
if lhs==1 then
  V1=gettempvar(1)
  V2=gettempvar(2)
  txt=['['+V1+','+V2+'] = '+'unix_g('+stk(top)(1)+')';
      'disp('+V1+')']
  stk=list(V1,'0','1','1','1')
else
  stk=list(list('unix_g('+stk(top)(1)+')','-1','?','1','10'),..
      list('unix_g('+stk(top)(1)+')','-1','1','1','1'))
end


