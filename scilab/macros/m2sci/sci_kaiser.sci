function [stk,txt,top]=sci_kaiser()
// Copyright INRIA
txt=[]
N=stk(top-1)(1)
beta=stk(top)(1)
stk=list('window'+rhsargs(['''hr''',N,beta])+'''','0',stk(top)(1),'1','1')

