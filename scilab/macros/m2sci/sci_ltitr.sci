function [stk,txt,top]=sci_ltitr()
// Copyright INRIA
txt=[]
// LTITR(A,B,U,X0)
A=stk(top-rhs+1)(1);na=stk(top-rhs+1)(4)
B=stk(top-rhs+2)(1)
U=stk(top-rhs+3)(1);U='('+U+').'''
nt=stk(top-rhs+3)(3)
nu=stk(top-rhs+3)(4)

if rhs==3 then
  stk=list('ltitr'+rhsargs([A,B,U])+'.''','0',nt,na,'1')
else	
  X0=stk(top)(1)
  stk=list('ltitr'+rhsargs([A,B,U,X0])+'.''','0',nt,na,'1')
end
endfunction
