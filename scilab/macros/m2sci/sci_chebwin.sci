function [stk,txt,top]=sci_chebwin()
// Copyright INRIA
txt=[]
N=stk(top-1)(1)
R=stk(top)(1)
stk=list('window'+rhsargs(['''ch''',N,R])+'''','0',stk(top)(1),'1','1')
endfunction
