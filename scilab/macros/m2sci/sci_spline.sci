function [stk,txt,top]=sci_spline()
// Copyright INRIA
txt=[]
XI=stk(top)(1)
X=stk(top-2)(1)
Y=stk(top-1)(1)
D='splin('+makeargs([X,Y])+')'
stk=list('interp('+makeargs([XI,X,Y,D])+')','0',stk(top)(3),stk(top)(4),'1')
