function [stk,txt,top]=sci_ss2tf()
// Copyright INRIA
txt=[]
[DEN,NUM]=lhsvarsnames()
if rhs==4 then
  a=stk(top-3)(1)
  b=stk(top-2)(1)
  c=stk(top-1)(1)
  d=stk(top)(1)
  iu='1'
else
  a=stk(top-4)(1)
  b=stk(top-3)(1)
  c=stk(top-2)(1)
  d=stk(top-1)(1)
  iu=stk(top)(1)
end
if ~isname(b) then
  b1=gettempvar(1)
  txt=[txt;b1+' = '+b]
  b=b1
end
b=b+'(:,'+iu+')'
if ~isname(d) then
  d1=gettempvar(2)
  txt=[txt;d1+' = '+d]
  d=d1
end
Ds=gettempvar(3)
d=d+'(:,'+iu+')'
txt=[txt;
    '%sm=simp_mode();simp_mode(%f)'
    '['+makeargs([Ds,NUM,DEN])+']  = ss2tf(syslin(''c'','+makeargs([a,b,c,d])+'))';
    DEN+' = '+DEN+'(1)';
    NUM+' = coeff('+NUM+',0:degree('+DEN+'))'
    NUM+' = '+NUM+'(:,$:-1:1)'
    DEN+' = coeff('+DEN+')'
    DEN+' = '+DEN+'($:-1:1)';
    'simp_mode(%sm)']
stk=list(list('?','-2','1','?','?'),list('?','-2','1','?','?'))
endfunction
