function [stk,txt,top]=sci_ss()
// Copyright INRIA
txt=[]
// sys=ss(A,B,C,D,Ts)
if stk(top-rhs+1)(5)=='1' then //SS(A,B,C,D,Ts) assumed
  dom='''c'''
  if rhs>=5 then 
    if stk(top-rhs+5)(5)=='1' then
      dom=stk(top-rhs+5)(1),
    else
      set_infos('fifth argument of ss ignored',2)
    end
  end
  if rhs>=4 then
    A=stk(top-rhs+1)(1)
    B=stk(top-rhs+2)(1)
    C=stk(top-rhs+3)(1)
    D=stk(top-rhs+4)(1)
    stk=list('syslin'+rhsargs([dom,A,B,C,D]),'0','?','?','15')
  elseif rhs==1 then
    D=stk(top-rhs+1)(1)
    stk=list('syslin'+rhsargs([dom,'[]','[]','[]',D]),'0','?','?','15')
  end
else //SYS = SS(SYS)
  RHS=[]
  for k=1:rhs
    RHS=[stk(top)(1),RHS]
    top=top-1
  end
  top=top+1
  set_infos(['ss(sys) syntax not yet handled';
      'matlab syntax used'],2)
  stk=list('ss'+rhsargs(RHS),'0',w(1,1),w(1,2),w(1,3))
end

