function [stk,txt,top]=sci_balance()
// Copyright INRIA
txt=[]
if lhs==1 then
  t1=gettempvar(1)
  t2=gettempvar(2)
  txt=lhsargs([t1,t2])+' = balanc('+stk(top)(1)+')'
  stk=list(t1,'0',stk(top)(3),stk(top)(4),'1')
else
  [t,b]=lhsvarsnames()
  txt='['+b+','+t+'] = balanc('+stk(top)(1)+')'
  stk=list(list('?','-2',stk(top)(3),stk(top)(4),'1'),..
      list('?','-2',stk(top)(3),stk(top)(4),'1'))
end

