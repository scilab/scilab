function [stk,txt,top]=sci_boxcar()
// Copyright INRIA
txt=[]
N=stk(top-1)(1)
stk=list('window'+rhsargs(['''re''',N])+'''','0',stk(top)(1),'1','1')

