function [stk,txt,top]=sci_kaiser()
// Copyright INRIA
txt=[]
N=stk(top-1)(1)
Beta=stk(top)(1)
stk=list('window'+rhsargs(['''hr''',N,Beta])+'''','0',stk(top)(1),'1','1')
endfunction
